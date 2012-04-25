/*
navdata.c - navigation data methods for AR.Drone autopilot program

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

#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#define CTRL_STATES_STRING
#include <control_states.h>

#include "autopilot.h"

/* Initialization local variables before event loop  */
inline C_RESULT demo_navdata_client_init( void* data ) {

  return C_OK;
}

/* Receving navdata during the event loop */
inline C_RESULT demo_navdata_client_process(const navdata_unpacked_t* const navdata ) {

        g_navdata = (navdata_demo_t *)&navdata->navdata_demo;

	return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT demo_navdata_client_release( void ) {

	return C_OK;
}

/* Registering to navdata client */
	BEGIN_NAVDATA_HANDLER_TABLE
NAVDATA_HANDLER_TABLE_ENTRY(demo_navdata_client_init, demo_navdata_client_process, demo_navdata_client_release, NULL)
	END_NAVDATA_HANDLER_TABLE

