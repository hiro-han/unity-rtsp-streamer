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
  RtstStreamer(const std::string url);
  ~RtstStreamer();

  bool Initialize();

  void push(const uint8_t *image, const size_t size);

 private:
  const static int kMaxQueueSize;
  void Run();
  static void MediaConfigure(GstRTSPMediaFactory *factory, GstRTSPMedia *media,
                             gpointer user_data);
  static void NeedData(GstElement *appsrc, guint unused, gpointer user_data);

  std::unique_ptr<uint8_t[]> pop();

  std::string url_;
  int width_;
  int height_;
  int fps_;
  std::thread thread_;
  uint64_t timestamp_;
  guint image_size_;
  std::mutex mutex_;
  std::queue<std::unique_ptr<uint8_t[]>> queue_;

  GMainLoop *loop_;
  GstRTSPServer *server_;
};

#endif