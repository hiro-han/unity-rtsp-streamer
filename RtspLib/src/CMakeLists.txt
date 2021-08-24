cmake_minimum_required(VERSION 2.8)

set(RTSPLIB rtsp-streamer-lib)

project(${RTSPLIB})

# gstreamer
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GSTREAMER_LIB REQUIRED  gstreamer-1.0 gstreamer-rtsp-1.0)
# # pkg_check_modules(GSTREAMERMM_LIB REQUIRED  gstreamer-1.0 gstreamer-rtsp-1.0)
# # pkg_check_modules(GSTREAMER_LIB REQUIRED  gstreamer-1.0)

# gstreamer rtsp
#file(GLOB rtspserverfiles gst-rtsp-server/gst/rtsp-server/*.c gst-rtsp-server/gst/rtsp-sink/*.c)
# include_directories(gst-rtsp-server)

# include_directories(gst-rtsp-server/gst/rtsp-server)

# message("GSTREAMER : ${GSTREAMERMM_CFLAGS_OTHER}" )
# target_include_directories(testGstreamer PUBLIC ${GSTREAMERMM_LIB_INCLUDE_DIRS})
# target_link_libraries(testGstreamer ${GSTREAMERMM_LIB_LIBRARIES})
# target_compile_options(testGstreamer PUBLIC ${GSTREAMERMM_CFLAGS_OTHER})


# find_package(GStreamer REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_library(${RTSPLIB} SHARED rtsp_streamer.cpp  ${rtspserverfiles})

# gstreamer
# target_include_directories(${RTSP} PRIVATE ${GSTREAMERMM_LIB_INCLUDE_DIRS})
# target_link_libraries(${RTSP} ${GSTREAMERMM_LIB_LIBRARIES})
# message("GSTREAMER : ${GSTREAMERMM_LIB_INCLUDE_DIRS}" )
# message("GSTREAMER : ${GSTREAMERMM_LIB_LIBRARIES}" )
target_include_directories(${RTSPLIB} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSPLIB} ${GSTREAMER_LIB_LIBRARIES})
# message("GSTREAMER : ${GSTREAMER_LIB_INCLUDE_DIRS}" )
# message("GSTREAMER : ${GSTREAMER_LIB_LIBRARIES}" )


# gstreamer rtsp
target_link_libraries(${RTSPLIB} libgstrtspserver-1.0.so)
# target_include_directories(${RTSPLIB} PRIVATE libgstrtspserver-1.0.so)
# target_include_directories(${RTSP} PRIVATE gst-rtsp-server/gst)
# include_directories(gst-rtsp-server/gst/rtsp-server)
# include_directories(gst-rtsp-server)

# message("GSTREAMER RTST: ${GSTREAMER_LIB_INCLUDE_DIRS}" )


target_link_libraries(${RTSPLIB} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_include_directories(${RTSPLIB} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

target_include_directories(${RTSPLIB} PRIVATE ../lib/plog/include)

# ${GSTREAMERMM_LIBRARIES}
#add_executable(a.out main.c)
#target_link_libraries(a.out calc)

#https://ochaochaocha3.hateblo.jp/entry/2017/08/09/gstreamer-tutorial-1-with-gstreamermm