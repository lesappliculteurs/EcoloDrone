#!/bin/sh
nc -l -p 50007 | ffmpeg -f rawvideo -pix_fmt bgr24 -s 320x240 -r 15 -i - -qscale 1 -an -f avi -r 15 -y foo.avi

