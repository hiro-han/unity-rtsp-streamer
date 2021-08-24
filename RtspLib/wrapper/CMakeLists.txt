cmake_minimum_required(VERSION 2.8)

set(RTSPCLIB rtsp-streamer-clib)

file(GLOB RTSP_SRC ${PROJECT_DIR}/src/*.cpp)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)


project(${RTSPCLIB})

add_library(${RTSPCLIB} SHARED wrapper.cpp ${RTSP_SRC})


target_include_directories(${RTSPCLIB} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})
target_link_libraries(${RTSPCLIB} ${GSTREAMER_LIB_LIBRARIES})
target_link_libraries(${RTSPCLIB} libgstrtspserver-1.0.so)
target_include_directories(${RTSPCLIB} PRIVATE ../lib/plog/include)


# RtspStreamerLib
# target_link_libraries(${RTSPCLIB} ${RTSPLIB})
target_include_directories(${RTSPCLIB} PRIVATE ${PROJECT_DIR}/src)

# gstreamer
target_link_libraries(${RTSPCLIB} ${GSTREAMER_LIB_LIBRARIES})
target_include_directories(${RTSPCLIB} PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})

# gstreamer rtsp
target_link_libraries(${RTSPCLIB} libgstrtspserver-1.0.so)
target_include_directories(${RTSPCLIB} PRIVATE ../lib/plog/include)

# OpenGL
target_link_libraries(${RTSPCLIB}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_include_directories(${RTSPCLIB}  PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
