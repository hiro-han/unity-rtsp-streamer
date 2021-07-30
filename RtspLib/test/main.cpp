#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "rtsp_streamer.h"

int main() {
  plog::init(plog::debug, "log.txt");

  cv::Mat img1 = cv::imread("../../test/data/1.jpg");
  cv::Mat img2 = cv::imread("../../test/data/2.jpg");
  cv::Mat img3 = cv::imread("../../test/data/3.jpg");
  if (img1.empty() || img2.empty() || img3.empty()) {
    return -1;
  }

  RtstStreamer rtsp_streamer(0, 0, "/test", img1.cols, img1.rows,
                             img1.elemSize(), 1);
  rtsp_streamer.Initialize();

  PLOG_DEBUG << "mat size: " << img1.total() * img1.elemSize();

  size_t size = img1.rows * img1.cols * img1.elemSize();

  while (true) {
    PLOG_DEBUG << "img1";
    rtsp_streamer.setImage(img1.data, size);
    usleep(100 * 1000);

    PLOG_DEBUG << "img2";
    rtsp_streamer.setImage(img2.data, size);
    usleep(100 * 1000);

    PLOG_DEBUG << "img3";
    rtsp_streamer.setImage(img3.data, size);
    usleep(100 * 1000);
  }

  return 0;
}