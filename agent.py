'''
greenball_agent.py - Python green-ball-tracking agent for AR.Drone autopilot program.

    To use this program with the AR.Drone Autopylot package, you should rename
    or symbolic-link the program as agent.py.

    Copyright (C) 2010 Simon D. Levy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of the 
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License 
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 You should also have received a copy of the Parrot Parrot AR.Drone 
 Development License and Parrot AR.Drone copyright notice and disclaimer 
 and If not, see 
   <https://projects.ardrone.org/attachments/277/ParrotLicense.txt> 
 and
   <https://projects.ardrone.org/attachments/278/ParrotCopyrightAndDisclaimer.txt>.

 Author gratefully acknowledges the help of Professor Joshua Stough in writing
 this program.
'''

from agent import *
import copy
import cv
import socket
import time
import math

# For OpenCV image display
WINDOW_NAME = 'Autopilot' 

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007
FRAME_TAU = 1/15.
# Routine called by C program.
def action(img_bytes, img_width, img_height, is_belly, ctrl_state, vbat_flying_percentage, theta, phi, psi, altitude, vx, vy, vz):
    print theta, phi, psi, altitude, vx, vy, vz, vbat_flying_percentage
    # Set up command defaults
    start = 0 
    select = 0 
    zap = 0
    enable = 0 
    phi = 0     
    theta = 0 
    gaz = 0
    yaw = 0
    zap = 0
    # Create full-color image from bytes
    full_image = create_image_header(img_width, img_height, 3) 
    cv.SetData(full_image, img_bytes, img_width*3)
    
    if not hasattr(action, 'sock'):
        action.last_frame = full_image
        action.last_t = time.time()
        action.last_time = time.time()
        #action.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        #action.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 32)
        HOST = 'localhost'    # The remote host
        PORT = 50007              # The same port as used by the server
        action.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        action.sock.connect((HOST, PORT))
        #s.sendall('Hello, world')
        action.last_frame = full_image
        fourcc = cv.CV_FOURCC('M','P','4','2')
        ##action.cv_writer = cv.CreateVideoWriter('out.avi', fourcc, 15, (img_width, img_height), 1)

    #Get the current time, to try and smooth out the framerate
    missed_frames = int(math.floor((time.time() - action.last_time) / FRAME_TAU))
    action.last_time = time.time()
    print "Missed: ", missed_frames
    #for i in xrange(missed_frames):
    #    cv.WriteFrame(action.cv_writer, action.last_frame)

    # Display full-color image
    if img_height == 240 and not hasattr(action, 'base_image'):
        action.base_image = cv.CloneImage(full_image)
    print cv.GetSize(full_image)
    if img_height != 240:
        #new_img = cv.CreateImage( ( 320, 240), cv.IPL_DEPTH_8U, 3)#cv.CreateMat(240, 320, cv.IPL_DEPTH_8U)#
        cv.Resize(full_image, action.base_image)
        full_image = action.base_image#new_img
    ##cv.WriteFrame(action.cv_writer, full_image)
    cv.NamedWindow(WINDOW_NAME, flags=cv.CV_WINDOW_NORMAL)
    cv.ShowImage(WINDOW_NAME, full_image)       
    
    str_img = full_image.tostring()
    for i in xrange(missed_frames):
        action.sock.sendall(action.last_frame.tostring())
    action.last_frame = full_image
    action.sock.sendall(str_img)

    cv.WaitKey(5)
    
    # Send control parameters back to drone (ignored)
    return (0, 0, 0, 0, 0, 0, 0, 0)

def create_image_header(img_width, img_height, channels):
    return cv.CreateImageHeader((img_width,img_height), cv.IPL_DEPTH_8U, channels)  

