cmake_minimum_required(VERSION 2.8)

project(rtsp-streamer)
find_package(OpenCV REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(rtsp-streamer main.cpp)

target_link_libraries(rtsp-streamer ${OpenCV_LIBS} )
target_link_libraries(rtsp-streamer rtsp-streamer-lib)
target_link_libraries(rtsp-streamer ${GSTREAMER_LIB_LIBRARIES})
target_link_libraries(rtsp-streamer libgstrtspserver-1.0.so)

# target_link_libraries(rtsp-streamer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})
# target_include_directories(rtsp-streamer PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(rtsp-streamer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_include_directories(rtsp-streamer PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})



target_include_directories(rtsp-streamer PRIVATE ../src)
target_include_directories(rtsp-streamer PRIVATE ${GSTREAMER_LIB_INCLUDE_DIRS})

target_include_directories(rtsp-streamer PRIVATE ../lib/plog/include)

