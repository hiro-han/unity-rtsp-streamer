cmake_minimum_required(VERSION 2.8)

set(RTSP rtsp-streamer)

project(${RTSP})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# gstreamer
find_package(PkgConfig REQUIRED)
# pkg_check_modules(GSTREAMERMM_LIB REQUIRED  gstreamer-1.0 gstreamer-rtsp-1.0)
pkg_check_modules(GSTREAMER_LIB REQUIRED  gstreamer-1.0 gstreamer-rtsp-1.0)
# pkg_check_modules(GSTREAMER_LIB REQUIRED  gstreamer-1.0)

# gstreamer rtsp
#file(GLOB rtspserverfiles gst-rtsp-server/gst/rtsp-server/*.c gst-rtsp-server/gst/rtsp-sink/*.c)
# include_directories(gst-rtsp-server)

# include_directories(gst-rtsp-server/gst/rtsp-server)

# message("GSTREAMER : ${GSTREAMERMM_CFLAGS_OTHER}" )
# target_include_directories(testGstreamer PUBLIC ${GSTREAMERMM_LIB_INCLUDE_DIRS})
# target_link_libraries(testGstreamer ${GSTREAMERMM_LIB_LIBRARIES})
# target_compile_options(testGstreamer PUBLIC ${GSTREAMERMM_CFLAGS_OTHER})


# find_package(GStreamer REQUIRED)

add_library(${RTSP} SHARED rtsp_streamer.cpp  ${rtspserverfiles})

# gstreamer
# target_include_directories(${RTSP} PRIVATE ${GSTREAMERMM_LIB_INCLUDE_DIRS})
# target_link_libraries(${RTSP} ${GSTREAMERMM_LIB_LIBRARIES})
# message("GSTREAMER : ${GSTREAMERMM_LIB_INCLUDE_DIRS}" )
# message("GSTREAMER : ${GSTREAMERMM_LIB_LIBRARIES}" )
target_include_directories(${RTSP} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSP} ${GSTREAMER_LIB_LIBRARIES})
# message("GSTREAMER : ${GSTREAMER_LIB_INCLUDE_DIRS}" )
# message("GSTREAMER : ${GSTREAMER_LIB_LIBRARIES}" )


# gstreamer rtsp
target_include_directories(${RTSP} PRIVATE libgstrtspserver-1.0.so)
# target_include_directories(${RTSP} PRIVATE gst-rtsp-server/gst)
# include_directories(gst-rtsp-server/gst/rtsp-server)
# include_directories(gst-rtsp-server)

# message("GSTREAMER RTST: ${GSTREAMER_LIB_INCLUDE_DIRS}" )



# ${GSTREAMERMM_LIBRARIES}
#add_executable(a.out main.c)
#target_link_libraries(a.out calc)

#https://ochaochaocha3.hateblo.jp/entry/2017/08/09/gstreamer-tutorial-1-with-gstreamermm