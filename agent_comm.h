/*
agent_comm.h - datatypes and method prototypes for cross-language communication in AR.Drone
               autopilot program

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
*/

typedef struct {
	int start; 
	int select; 
	int zap; 
	int enable; 
	float phi; 
	float theta; 
	float gaz; 
	float yaw;
} commands_t;

void agent_comm_init();

void agent_comm_act(unsigned char * img_bytes, int img_width, int img_height, bool_t img_is_belly,
	            navdata_demo_t * navdata, commands_t * commands);

void agent_comm_close();



