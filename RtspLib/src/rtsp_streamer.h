#ifndef RTSP_STREAMER_H_
#define RTSP_STREAMER_H_

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include <mutex>
#include <queue>
#include <string>
#include <thread>

class RtstStreamer {
 public:
  RtstStreamer(const int mode, const int format, const std::string &url,
               const int width, const int height, const int depth,
               const int fps);
  ~RtstStreamer();

  bool Initialize();

  void setImage(const uint8_t *image, const size_t size);

  static void MediaConfigure(GstRTSPMediaFactory *factory, GstRTSPMedia *media,
                             gpointer user_data);
  static void NeedData(GstElement *appsrc, guint unused, gpointer user_data);

 private:
  void Run();

  static const std::string kH264;
  static const std::string kJpeg;

  std::string url_;
  int width_;
  int height_;
  int depth_;
  int fps_;
  int mode_;
  int format_;
  std::thread thread_;
  uint64_t timestamp_;
  guint image_size_;
  std::mutex mutex_;
  std::unique_ptr<uint8_t[]> buffer_;

  GMainLoop *loop_;
  GstRTSPServer *server_;
};

#endif