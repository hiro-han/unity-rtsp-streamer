#include "wrapper.h"
#include "rtsp_streamer.h"

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

extern "C" {

RtstStreamer* pRtspStreamer;

void Initialize(int format, char* url, int width, int height, int depth) {
  plog::init(plog::debug, "log.txt");

  pRtspStreamer = new RtstStreamer(0, format, url, width, height, depth, 1);
  pRtspStreamer->Initialize();

  PLOG_DEBUG << "Exit : Initialize";
}

void SetImage(void* pImage, int size) {
  PLOG_DEBUG << "Eenter : SetImage";
  pRtspStreamer->setImage((uint8_t*)pImage, size);
}

void Finalize() { delete pRtspStreamer; }
}