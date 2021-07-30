#include "rtsp_streamer.h"
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>
#include <cstring>
#include <fstream>
#include <memory>

const std::string RtstStreamer::kH264 =
    "( appsrc name=mysrc ! videoconvert ! "
    "x264enc ! speed-preset=ultrafast tune=zerolatency threads=1 ! "
    "rtph264pay name=pay0 pt=96 )";

const std::string RtstStreamer::kJpeg =
    "( appsrc name=mysrc ! videoconvert ! "
    "video/x-raw,format=I420 ! jpegenc ! rtpjpegpay name=pay0 pt=96 )";

RtstStreamer::RtstStreamer(const Encode enc, const ImageFormat format,
                           const std::string &url, const int width,
                           const int height, const int depth, const int fps)
    : enc_(enc),
      format_(format),
      url_(url),
      width_(width),
      height_(height),
      depth_(depth),
      fps_(fps) {}

RtstStreamer::~RtstStreamer() {}

bool RtstStreamer::Initialize() {
  thread_ = std::thread(&RtstStreamer::Run, this);
  return true;
}

void RtstStreamer::Run() {
  PLOG_DEBUG << "Enter Run()";

  gst_init(NULL, NULL);

  loop_ = g_main_loop_new(NULL, false);
  server_ = gst_rtsp_server_new();

  GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server_);
  GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

  switch (enc_) {
    case Encode::kJPEG:
      gst_rtsp_media_factory_set_launch(factory, kJpeg.c_str());
      break;
    case Encode::kH264:
      gst_rtsp_media_factory_set_launch(factory, kH264.c_str());
      break;
    default:
      gst_rtsp_media_factory_set_launch(factory, kJpeg.c_str());
      PLOG_WARNING << "Encode is not defined. Encode is set to BGR.";
  }

  g_signal_connect(factory, "media-configure",
                   (GCallback)&RtstStreamer::MediaConfigure, (gpointer) this);
  gst_rtsp_mount_points_add_factory(mounts, url_.c_str(), factory);
  g_object_unref(mounts);

  int id = gst_rtsp_server_attach(server_, NULL);
  if (id < 0) {
    PLOG_ERROR << "Failed gst_rtsp_server_attach(). id = " << id;
  }
  g_main_loop_run(loop_);
  PLOG_DEBUG << "Exit Run()";
}

void RtstStreamer::MediaConfigure(GstRTSPMediaFactory *factory,
                                  GstRTSPMedia *media, gpointer user_data) {
  PLOG_DEBUG << "Enter MediaConfigure()";

  RtstStreamer *rtsp_streamer = reinterpret_cast<RtstStreamer *>(user_data);

  GstElement *element = gst_rtsp_media_get_element(media);
  GstElement *appsrc =
      gst_bin_get_by_name_recurse_up(GST_BIN(element), "mysrc");

  switch (rtsp_streamer->format_) {
    case ImageFormat::kBGR:
      gst_util_set_object_arg(G_OBJECT(appsrc), "format", "time");
      g_object_set(G_OBJECT(appsrc), "caps",
                   gst_caps_new_simple(
                       "video/x-raw", "format", G_TYPE_STRING, "BGR", "width",
                       G_TYPE_INT, rtsp_streamer->width_, "height", G_TYPE_INT,
                       rtsp_streamer->height_, "framerate", GST_TYPE_FRACTION,
                       rtsp_streamer->fps_, 1, NULL),
                   NULL);
      break;
    case ImageFormat::kBGRA:
      gst_util_set_object_arg(G_OBJECT(appsrc), "format", "time");
      // g_object_set(G_OBJECT(appsrc), "blocksize", 196608, NULL);
      g_object_set(G_OBJECT(appsrc), "caps",
                   gst_caps_new_simple(
                       "video/x-raw", "format", G_TYPE_STRING, "BGRA", "width",
                       G_TYPE_INT, rtsp_streamer->width_, "height", G_TYPE_INT,
                       rtsp_streamer->height_, "framerate", GST_TYPE_FRACTION,
                       rtsp_streamer->fps_, 1, NULL),
                   NULL);
      break;
    default:
      gst_util_set_object_arg(G_OBJECT(appsrc), "format", "time");
      g_object_set(G_OBJECT(appsrc), "caps",
                   gst_caps_new_simple(
                       "video/x-raw", "format", G_TYPE_STRING, "BGR", "width",
                       G_TYPE_INT, rtsp_streamer->width_, "height", G_TYPE_INT,
                       rtsp_streamer->height_, "framerate", GST_TYPE_FRACTION,
                       rtsp_streamer->fps_, 1, NULL),
                   NULL);
      PLOG_WARNING << "Format is not defined. Format is set to BGR.";
  }

  rtsp_streamer->timestamp_ = 0;
  rtsp_streamer->image_size_ =
      rtsp_streamer->width_ * rtsp_streamer->height_ * rtsp_streamer->depth_;

  rtsp_streamer->mutex_.lock();
  rtsp_streamer->buffer_ =
      std::make_unique<uint8_t[]>(rtsp_streamer->image_size_);
  rtsp_streamer->mutex_.unlock();

  g_signal_connect(appsrc, "need-data", (GCallback)&RtstStreamer::NeedData,
                   (gpointer)user_data);
  gst_object_unref(appsrc);
  gst_object_unref(element);

  PLOG_DEBUG << "size = " << rtsp_streamer->image_size_;
  PLOG_DEBUG << "Exit MediaConfigure()";
}

void RtstStreamer::NeedData(GstElement *appsrc, guint unused,
                            gpointer user_data) {
  PLOG_DEBUG << "Enter NeedData()";

  RtstStreamer *rtsp_streamer = reinterpret_cast<RtstStreamer *>(user_data);
  GstBuffer *buffer =
      gst_buffer_new_allocate(NULL, rtsp_streamer->image_size_, NULL);
  if (buffer == NULL) {
    PLOG_ERROR << "Failed  gst_buffer_new_allocate()";
    return;
  }

  rtsp_streamer->mutex_.lock();
  gsize copied_size = gst_buffer_fill(buffer, 0, rtsp_streamer->buffer_.get(),
                                      rtsp_streamer->image_size_);
  rtsp_streamer->mutex_.unlock();

  if (rtsp_streamer->image_size_ != copied_size) {
    PLOG_ERROR << "Failed. data size = " << rtsp_streamer->image_size_
               << ", copied size = " << copied_size;
    return;
  }

  guint64 duration =
      gst_util_uint64_scale_int(1, GST_SECOND, rtsp_streamer->fps_);
  GST_BUFFER_PTS(buffer) = rtsp_streamer->timestamp_;
  GST_BUFFER_DURATION(buffer) = duration;

  rtsp_streamer->timestamp_ = rtsp_streamer->timestamp_ + duration;
  PLOG_DEBUG << "timestamp " << rtsp_streamer->timestamp_;
  PLOG_DEBUG << "duration " << duration;

  GValue ret;
  g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
  gst_buffer_unref(buffer);

  PLOG_DEBUG << "Exit NeedData()";
}

void RtstStreamer::setImage(const uint8_t *image, const size_t size) {
  PLOG_DEBUG << "Enter setImage()";
  mutex_.lock();
  if (buffer_) {
    std::memcpy(buffer_.get(), image, size);
    PLOG_DEBUG << "Copied image";
  }
  mutex_.unlock();
  PLOG_DEBUG << "Exit setImage()";
}
