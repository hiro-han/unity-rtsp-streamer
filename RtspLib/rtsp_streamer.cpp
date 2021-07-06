#include "rtsp_streamer.h"
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <cstring>
#include <memory>

const int RtstStreamer::kMaxQueueSize = 10;

RtstStreamer::RtstStreamer(const std::string url) : url_(url) {}

RtstStreamer::~RtstStreamer() {}

bool RtstStreamer::Initialize() {
  thread_ = std::thread(&RtstStreamer::Run, this);
  //  thread_(std::thread(&RtstStreamer::Run, this));
  return true;
}

void RtstStreamer::Run() {
  // initialize the GStreamer library
  gst_init(NULL, NULL);
  // Creates a new GMainLoop structure.   g_main_loop_new(GMainContext *context,
  // gboolean is_running)
  loop_ = g_main_loop_new(NULL, FALSE);
  // Create a new GstRTSPServer instance.
  server_ = gst_rtsp_server_new();

  GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server_);
  GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();
  gst_rtsp_media_factory_set_launch(factory,
                                    "( appsrc name=mysrc ! videoconvert ! "
                                    "x264enc ! rtph264pay name=pay0 pt=96 )");

  g_signal_connect(factory, "media-configure",
                   (GCallback)&RtstStreamer::MediaConfigure, this);
  gst_rtsp_mount_points_add_factory(mounts, url_.c_str(), factory);
  g_object_unref(mounts);

  // Attaches server to context    gst_rtsp_server_attach(GstRTSPServer *
  // server, GMainContext * context)
  int id = gst_rtsp_server_attach(server_, NULL);
  if (id < 0) {
    // error
  }
  g_main_loop_run(loop_);
}

void RtstStreamer::MediaConfigure(GstRTSPMediaFactory *factory,
                                  GstRTSPMedia *media, gpointer user_data) {
  RtstStreamer *rtsp_streamer = (RtstStreamer *)user_data;
  GstElement *element = gst_rtsp_media_get_element(media);
  GstElement *appsrc =
      gst_bin_get_by_name_recurse_up(GST_BIN(element), "mysrc");

  gst_util_set_object_arg(G_OBJECT(appsrc), "format", "time");
  g_object_set(
      G_OBJECT(appsrc), "caps",
      gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "RGB8",
                          "width", G_TYPE_INT, rtsp_streamer->width_, "height",
                          G_TYPE_INT, rtsp_streamer->height_, "framerate",
                          GST_TYPE_FRACTION, rtsp_streamer->fps_, 1, NULL),
      NULL);

  rtsp_streamer->timestamp_ = 0;
  rtsp_streamer->image_size_ =
      rtsp_streamer->width_ * rtsp_streamer->height_ * 3;
  g_signal_connect(appsrc, "need-data", (GCallback)&RtstStreamer::NeedData,
                   rtsp_streamer);
  gst_object_unref(appsrc);
  gst_object_unref(element);
}

void RtstStreamer::NeedData(GstElement *appsrc, guint unused,
                            gpointer user_data) {
  RtstStreamer *rtsp_streamer = (RtstStreamer *)user_data;
  GstBuffer *buffer =
      gst_buffer_new_allocate(NULL, rtsp_streamer->image_size_, NULL);
  if (buffer == NULL) {
    // error
    return;
  }

  // Copies size bytes from src to buffer at offset. gst_buffer_fill(GstBuffer *
  // buffer, gsize offset, gconstpointer
  // src, gsize size)
  std::unique_ptr<uint8_t[]> data = rtsp_streamer->pop();
  if (data == 0) {
    return;
  }
  gsize copied_size =
      gst_buffer_fill(buffer, 0, data.get(), rtsp_streamer->image_size_);
  data.reset();
  if (rtsp_streamer->image_size_ != copied_size) {
    // error
    return;
  }

  // // Fill buf with size bytes with val starting from offset. gsize
  // gst_buffer_memset (GstBuffer *buffer, gsize
  // offset,
  // // guint8 val, gsize size);
  // gsize filled_size = gst_buffer_memset(buffer, 0, 0xff, size);
  // if (size != filled_size) {
  //   // error
  //   return;
  // }

  //  ctx->white = !ctx->white;

  // The presentation timestamp (pts) in nanoseconds (as a GstClockTime) of the
  // data in the buffer. This is the
  // timestamp when the media should be presented to the user.
  //  GST_BUFFER_PTS(buffer) = ctx->timestamp;

  // he duration in nanoseconds (as a GstClockTime) of the data in the buffer
  // GST_BUFFER_DURATION(buffer)

  // Scale val by the rational number num / denom , avoiding overflows and
  // underflows and without loss of precision. num
  // must be non-negative and denom must be positive. guint64
  // gst_util_uint64_scale_int(guint64 val, gint num, gint
  // denom)
  //  GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND,
  //  fps_);
  guint64 duration =
      gst_util_uint64_scale_int(1, GST_SECOND, rtsp_streamer->fps_);
  GST_BUFFER_PTS(buffer) = rtsp_streamer->timestamp_;
  GST_BUFFER_DURATION(buffer) = duration;

  rtsp_streamer->timestamp_ = rtsp_streamer->timestamp_ + duration;

  GValue ret;
  g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
  gst_buffer_unref(buffer);
}

void RtstStreamer::push(const uint8_t *image, const size_t size) {
  std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(size);
  std::memcpy(buffer.get(), image, size);
  mutex_.lock();
  if (queue_.size() > kMaxQueueSize) {
    queue_.pop();
  }
  queue_.push(std::move(buffer));
  mutex_.unlock();
}

std::unique_ptr<uint8_t[]> RtstStreamer::pop() {
  std::unique_ptr<uint8_t[]> image;
  mutex_.lock();
  if (queue_.empty()) {
    // image = 0;
  } else {
    image = std::move(queue_.front());
  }
  mutex_.unlock();
  return image;
}