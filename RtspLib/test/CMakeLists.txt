cmake_minimum_required(VERSION 2.8)

set(RTSPLIBTEST rtsp-streamer-test)
project(${RTSPLIBTEST})

find_package(OpenCV REQUIRED)

add_executable(${RTSPLIBTEST} main.cpp)

# RtspLib
target_link_libraries(${RTSPLIBTEST} rtsp-streamer-lib)
target_include_directories(${RTSPLIBTEST} PRIVATE ${PROJECT_DIR}/src)

# OpenCV
target_link_libraries(${RTSPLIBTEST} ${OpenCV_LIBS})

# gstreamer
target_include_directories(${RTSPLIBTEST} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSPLIBTEST} ${GSTREAMER_LIB_LIBRARIES})

# gstreamer rtsp
target_link_libraries(${RTSPLIBTEST} libgstrtspserver-1.0.so)

# plog
target_include_directories(${RTSPLIBTEST} PRIVATE ../lib/plog/include)









