# unity-rtsp-streamer

This application outputs RTSP stream of unity camera.

## Environment
I developed and tested this applicaton on following environment.
- Ubuntu 16.04
- GStreamer 1.0
- gst-rtsp-server 1.8.3 
- Unity 2019.4.17f1


## Installation
I could not install the gst-rtsp-server using apt command. So, I built the gst-rtsp-server from source code.

1. Clone this repository  
2. Install gstreamer
```
$ sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

$ sudo apt-get install autoconf gtk-doc-tools
```
3. [Build gst-rtsp-server](RtspLib/README.md)
4. [Build RtspLib](RtspLib/README.md)
5. Copy RtspLib
```
$ cp RtspLib/build/wrapper/librtsp-streamer-clib.so Assets/Plugins/x86_64/ubuntu
```
## Build and Run
1. Build and Run unity application    
URL of RTSP stream is *rtsp://127.0.0.1:8554/test* as default.

# Use RtspStreamer in your appliation
1. Build and copy ***librtsp-streamer-clib.so*** to ***Assets/Plugins*** directory in your application.
2. Copy [***RtspStreamer.cs***](Assets/Scripts/RtspStreamer.cs) to ***Assets/Scripts*** directory.
3. Launch unity.
4. Go to the Hierarchy view, then add Game Object.
5. Drag & Drop RtspStreamer.cs to the Game Object.
6. Go to the Inspectore view of the Game Object, then set Render Texture, Fps and Url.