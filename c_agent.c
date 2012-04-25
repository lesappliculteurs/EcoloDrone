#include <navdata_common.h>

#include <stdio.h>

#include "agent_comm.h"

void agent_comm_init() {

}

void agent_comm_act(unsigned char * img_bytes, int img_width, int img_height, bool_t img_is_belly,
	            navdata_demo_t * navdata, commands_t * commands) {

	// Report navigation data
	printf("ctrl state=%6d battery=%2d%% theta=%+f phi=%+f psi=%+f altitude=%+3d vx=%+3d vy=%+3d vz=%+3d\n", 
		navdata->ctrl_state, navdata->vbat_flying_percentage, 
		navdata->theta, navdata->phi, navdata->psi, (int)navdata->altitude, 
		(int)navdata->vx, (int)navdata->vy, (int)navdata->vz);                     

	
	// Set up commands for a clockwise turn
	commands->start = 0; 
	commands->select = 0; 
	commands->zap = 0;
	commands->enable = 1; 
	commands->phi = 0; 	
	commands->theta = 0; 
	commands->gaz = 0; 
	commands->yaw = 1.0;
}

void agent_comm_close() {

}

