cmake_minimum_required(VERSION 2.8)
project(rtsp-streamer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER_LIB REQUIRED  gstreamer-1.0 gstreamer-rtsp-1.0)

add_subdirectory(src)
add_subdirectory(wrapper)
add_subdirectory(test)
