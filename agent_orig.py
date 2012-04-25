'''
agent.py - Python agent stub for AR.Drone autopilot program.

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
'''

from agent import *

# For OpenCV
#import cv
#WINDOW_NAME = "AR.Drone"

def action(img_bytes, img_width, img_height, is_belly, ctrl_state, vbat_flying_percentage, theta, phi, psi, altitude, vx, vy, vz):

	# Report navigation data
	print 'ctrl state=%6d battery=%2d%% theta=%+f phi=%+f psi=%+f altitude=%+3d vx=%+3d vy=%+3d vz=%+3d' % \
	      (ctrl_state, vbat_flying_percentage, theta, phi, psi, altitude, int(vx), int(vy), int(vz))

	# Show camera image using OpenCV
	#img = cv.CreateImageHeader((img_width,img_height), cv.IPL_DEPTH_8U, 3)
	#cv.SetData(img, img_bytes, 3*img_width)
	#cv.NamedWindow(WINDOW_NAME)
	#cv.ShowImage(WINDOW_NAME, img)
	#cv.WaitKey(5)

	# Set up commands for a clockwise turn
	start = 0; 
	select = 0; 
	zap = 0
	enable = 1; 
	phi = 0; 	
	theta = 0; 
	gaz = 0; 
	yaw = 1.0

	return (start, select, zap, enable, phi, theta, gaz, yaw)
