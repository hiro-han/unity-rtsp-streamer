cmake_minimum_required(VERSION 2.8)

set(RTSPCLIB rtsp-streamer-clib)

file(GLOB RTSP_SRC ${PROJECT_DIR}/src/*.cpp)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)


project(${RTSPCLIB})

add_library(${RTSPCLIB} SHARED wrapper.cpp ${RTSP_SRC})

# RtspLib
target_link_libraries(${RTSPCLIB} rtsp-streamer-lib)
target_include_directories(${RTSPCLIB} PRIVATE ${PROJECT_DIR}/src)

# gstreamer
target_include_directories(${RTSPCLIB} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSPCLIB} ${GSTREAMER_LIB_LIBRARIES})

# gstreamer rtsp
target_link_libraries(${RTSPCLIB} libgstrtspserver-1.0.so)

# plog
target_include_directories(${RTSPCLIB} PRIVATE ../lib/plog/include)

