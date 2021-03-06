/*
matlab_agent.c - C/Matlab communication code for AR.Drone autopilot agent.

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

#undef _GNU_SOURCE

#include <navdata_common.h>

#include "agent_comm.h"

#define FUNCTION_NAME "agent"

static Engine *ep;

static void put_variable(const char * varname, mxArray * array) {

	engPutVariable(ep, varname, array);
}

mxArray * create_numeric_array(mwSize size, mxClassID classid) {

	return mxCreateNumericMatrix(1, size, classid, 0); // 0 = not complex
}

void agent_comm_init() {

	if (!(ep = engOpen("\0"))) {
		fprintf(stderr, "\nCan't start MATLAB engine\n");	
		return;
	}
}

void agent_comm_act(unsigned char * img_bytes, int img_width, int img_height, bool_t img_is_belly,
	            navdata_demo_t * navdata, commands_t * commands) {

	// Need engine
	if (!ep) {
		return;
	}

	// Create flat matrix big enough to hold WIDTH X HEIGHT X 3 (RGB) image data
	mxArray * img =  create_numeric_array(img_height*img_width*3, mxUINT8_CLASS);

	// Copy image bytes to matrix in order appropriate for reshaping
	unsigned char * p = (unsigned char *)mxGetChars(img);
	int rgb, row, col;;
	for (rgb=0; rgb<3; ++rgb) {
		for (col=0; col<img_width; ++col) {
			for (row=0; row<img_height; ++row) {
				*p = img_bytes[2-rgb + 3*(row * img_width +  col)]; 
				p++;
			}
		}
	}

	// Put the image variable into the Matlab environment
	put_variable("img", img);

	// Create a variable for passing navigation data to Matlab function
	mxArray * mx_navdata = create_numeric_array(10, mxDOUBLE_CLASS);
	
	// Build command using reshaped IMG variable and constants from navdata structure
	char cmd[200];
	double * np = (float *)mxGetData(mx_navdata);
	*np++ = (float)(img_is_belly?1:0);
	*np++ = (float)navdata->ctrl_state; 	     
	*np++ = (float)navdata->vbat_flying_percentage;
	*np++ = navdata->theta;                
	*np++ = navdata->phi;                 
	*np++ = navdata->psi;                
	*np++ = navdata->altitude;    	    
	*np++ = navdata->vx;               
	*np++ = navdata->vy;              
	*np   = navdata->vz;

	// Put the navdata variable into the Matlab environment
	put_variable("navdata",   mx_navdata);

	// Build and evaluate command
	sprintf(cmd,"commands = %s(reshape(img, %d, %d, 3), navdata);", FUNCTION_NAME, img_height, img_width);
	if (engEvalString(ep, cmd)) {
		fprintf(stderr, "Error evaluating command: %s\n", cmd);
	}


	// Get output variables
	double *cp = (double *)mxGetData(engGetVariable(ep, "commands"));

	commands->start   = (int)cp[0];
	commands->select  = (int)cp[1];
	commands->zap     = (int)cp[2];
	commands->enable  = (int)cp[3];
	commands->phi     = cp[4];
	commands->theta   = cp[5];
	commands->gaz     = cp[6];
	commands->yaw     = cp[7];

	// Deallocate memory
	mxDestroyArray(img);
	mxDestroyArray(mx_navdata);
}


void agent_comm_close() {

	engEvalString(ep, "close;");
	engClose(ep);
}
