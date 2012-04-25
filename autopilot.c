/*
autopilot.c - ardrone_tool custom code for AR.Drone autopilot agent.

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

#include <stdio.h>

//ARDroneLib
#include <ardrone_tool/ardrone_time.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <ardrone_tool/Control/ardrone_control.h>
#include <ardrone_tool/UI/ardrone_input.h>

//Common
#include <config.h>
#include <ardrone_api.h>

//VP_SDK
#include <ATcodec/ATcodec_api.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Api/vp_api_thread_helper.h>
#include <VP_Os/vp_os_signal.h>
#include <VP_Os/vp_os_types.h>

//Local project
#define _MAIN
#include "autopilot.h"
#include "agent.h"

#define PORT 40000

PROTO_THREAD_ROUTINE(video_stage, data);

extern input_device_t gamepad;

/* Implementing Custom methods for the main function of an ARDrone application */

/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(int argc, char **argv) {

	/* Registering for a new device of game controller */
	ardrone_tool_input_add( &gamepad );

	/* Don't exit till indicated */
	g_exit = FALSE;

	/* Start all threads of your application */
	START_THREAD( video_stage, NULL );

	/* Start with autopilot off */
	g_autopilot = FALSE;

	/* Start with front camera */
	g_bellycam = FALSE;
	ardrone_at_zap(0);

	return C_OK;
}

/* The delegate object calls this method when the event loop exits */
C_RESULT ardrone_tool_shutdown_custom() {

	/* Relinquish all threads of your application */
	JOIN_THREAD( ardrone_control );
	JOIN_THREAD( navdata_update );
	JOIN_THREAD( video_stage );

	/* Unregistering for the current device */
	ardrone_tool_input_remove( &gamepad );

	return C_OK;
}

C_RESULT signal_exit() {

	return C_OK;
}

/* Implementing thread table in which you add routines of your application and those provided by the SDK */
	BEGIN_THREAD_TABLE
	THREAD_TABLE_ENTRY( ardrone_control, 20 )
	THREAD_TABLE_ENTRY( navdata_update, 20 )
	THREAD_TABLE_ENTRY( video_stage, 20 )
	END_THREAD_TABLE

