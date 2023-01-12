# Virtual-Drawing-Pad #

A simple application of computer vision using OpenCV library in ubuntu 14.04 LTS. Also works in Ubuntu 16.04.

## Installation ##

1.  OS: Ubuntu 14.04 or 16.04 (Tested for these two)
2.  OpenCV Library: https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html

## Compile Project ##
```bash
$ g++ -ggdb vdp.cpp -o obj `pkg-config --cflags --libs opencv`
```

## Run Project ##
```bash
$ ./obj
```
## How does the program work ? ##

The program uses thresholding function to detect marker in its environment.Then centroid is
determined using moments of image. The value corresponding to pixel is changed giving desired color.

## What can the program do ? ## 

1.  Marker size can be increased to give strokes of different width.
2.  Any color can be chosen for the stroke.
3.  Eraser can be used with variable radius.
4.  Marker start drawing only when it is within certain distance from camera.
5.  You can easily save the drawing.

Enjoy Drawing!
