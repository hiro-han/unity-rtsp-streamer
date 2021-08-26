# RtspLib
C++ library and application to output RTSP stream.
Also, this library includes wrapper for c#.

## Install gst-rtsp-server
1. Clone github repository
```
$ git submodule update --init
```

Following repositories are cloned under lib directory.
- https://github.com/GStreamer/gst-rtsp-server.git
    - SHA-1 : af5bacd59a393ca359ee727cb86900a4974cf537
    - Tag : refs/tags/1.8.3
- https://github.com/SergiusTheBest/plog.git
    - SHA-1 : 914e799d2b08d790f5d04d1c46928586b3a41250

2. Build and Install
```
$ cd gst-rtsp-server
$ ./autogen.sh
$ ./configure
$ ./make
$ sudo make install
```
(*) If you can install gst-rtsp-server using apt command, you can use following command.
```
$ sudo apt-get install libgstrtspserver-1.0-dev gstreamer1.0-rtsp
```

## Build 
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run test application
1. Put test data
Put test image data under test/data directory.
All files should have same resolution.
- File name:
    - 1.jpg
    - 2.jpg
    - 3.jpg

If you want to change data directory, file name and number of files, you can edit [test/main.cpp](test/main.cpp)

2. Run test application
```
$ cd build/test
$ ./rtsp-streamer-test
```
URL of RTSP stream is *rtsp://127.0.0.1:8554/test* as default.
