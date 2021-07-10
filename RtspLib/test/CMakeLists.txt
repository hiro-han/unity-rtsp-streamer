cmake_minimum_required(VERSION 2.8)

project(rtsp-streamer)
find_package(OpenCV REQUIRED)

add_executable(rtsp-streamer main.cpp)

target_link_libraries(rtsp-streamer ${OpenCV_LIBS} )
target_link_libraries(rtsp-streamer rtsp-streamer-lib)
target_link_libraries(rtsp-streamer ${GSTREAMER_LIB_LIBRARIES})
target_link_libraries(rtsp-streamer libgstrtspserver-1.0.so)


target_include_directories(rtsp-streamer PRIVATE ../src)
target_include_directories(rtsp-streamer PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})

target_include_directories(rtsp-streamer PRIVATE ../lib/plog/include)

