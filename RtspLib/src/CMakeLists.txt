cmake_minimum_required(VERSION 2.8)

set(RTSPLIB rtsp-streamer-lib)
project(${RTSPLIB})

# Build library
add_library(${RTSPLIB} SHARED rtsp_streamer.cpp)

# gstreamer
target_include_directories(${RTSPLIB} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSPLIB} ${GSTREAMER_LIB_LIBRARIES})

# gstreamer rtsp
target_link_libraries(${RTSPLIB} libgstrtspserver-1.0.so)

# plog
target_include_directories(${RTSPLIB} PRIVATE ../lib/plog/include)
