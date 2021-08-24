# Intall gstreamer
Intall gstreamer on Host PC

## Install gstreamer
```
$ sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```
## Install gst-rtsp-server
I failed to install gst-rtsp-server. So, I built it from source code.

### Failed
```
$ sudo apt-get install libgstrtspserver-1.0-dev gstreamer1.0-rtsp
```

### Suceeded
#### Intall libraries
```
$ sudo apt-get install autoconf gtk-doc-tools
```

#### Clone github repository
```
$ git submodule update --init
```

Following repositories are cloned under lib directory.
- https://github.com/GStreamer/gst-rtsp-server.git
    - SHA-1 : af5bacd59a393ca359ee727cb86900a4974cf537
    - Tag : refs/tags/1.8.3
- https://github.com/SergiusTheBest/plog.git
    - SHA-1 : 914e799d2b08d790f5d04d1c46928586b3a41250

#### Build
```
$ cd gst-rtsp-server
$ git checkout -b 1.8.3 

$ ./autogen.sh
$ ./configure
$ ./make
$ sudo make install
```

# Build
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Run
## Put test data
Put test image data under test/data directory.
All files should have same resolution.
- File name:
    - 1.jpg
    - 2.jpg
    - 3.jpg

If you want to change data directory, file name and number of files, you can edit [test/main.cpp](test/main.cpp)

### Test App
```
$ cd build/test
$ ./rtsp-streamer-test
```
