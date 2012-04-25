#!/bin/sh
nc -l -p 50007 | ffmpeg -f rawvideo -pix_fmt bgr24 -s 320x240 -r 15 -i - -an -r 15 -vcodec libx264   -acodec libmp3lame -ab 32k -threads 0 -f flv "rtmp://live.justin.tv/app/KEY" -an -f avi -r 15 foo.avi
