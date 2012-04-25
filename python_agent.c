/*
python_agent.c - C/Python communication code for AR.Drone autopilot agent.

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

#include <Python.h>

#undef _GNU_SOURCE

#include <navdata_common.h>

#include <stdio.h>
#include <stdlib.h>

#include "agent_comm.h"

#define AGENT_MODULE_NAME "agent"
#define AGENT_FUNCTION_NAME "action"

static PyObject *pModule, *pFunc, *pArgs, *pResult;

static void error(const char * msg, const char * name) {
	
	fprintf(stderr, msg, name);
	exit(1);
}

static void fun_error(const char * msg, const char * name) {
	PyErr_Print();
	Py_DECREF(pArgs);
	Py_DECREF(pResult);
	Py_DECREF(pModule);
	Py_DECREF(pFunc);
	error(msg, name);
}

void agent_comm_init() {

	Py_Initialize();

	PyObject * pName = PyString_FromString(AGENT_MODULE_NAME);

	pModule = PyImport_Import(pName);

	Py_DECREF(pName);

	if (pModule == NULL) {
		PyErr_Print();
		error("Failed to load %s", AGENT_MODULE_NAME);
	}

	pFunc = PyObject_GetAttrString(pModule, AGENT_FUNCTION_NAME);

	if (!(pFunc && PyCallable_Check(pFunc))) {
		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		error("Cannot find function %s", AGENT_FUNCTION_NAME);
	}

	pArgs = PyTuple_New(13);

	pResult = PyTuple_New(8);
}

static void set_arg(PyObject * pValue, int pos) {

	if (!pValue) {
		fun_error("Cannot convert argument", "");
	}
	
	PyTuple_SetItem(pArgs, pos, pValue);
}

static void set_int_arg(int val, int pos) {

        set_arg(PyInt_FromLong((long)val), pos);
}

static void set_float_arg(float val, int pos) {

	set_arg(PyFloat_FromDouble((double)val), pos);
}

static int get_int_result(PyObject *pResult, int pos) {

	return (int)PyInt_AsLong(PyTuple_GetItem(pResult, pos));
}

static float get_float_result(PyObject *pResult, int pos) {

	return (float)PyFloat_AsDouble(PyTuple_GetItem(pResult, pos));
}

void agent_comm_act(unsigned char * img_bytes, int img_width, int img_height, bool_t img_is_belly,
		    navdata_demo_t * navdata, commands_t * commands) {

        PyObject *pImageBytes = PyByteArray_FromStringAndSize((const char *)img_bytes, img_width*img_height*3);

	int k = 0;

        set_arg(pImageBytes,                         k++);
	set_int_arg(img_width, 	                     k++);
	set_int_arg(img_height,	                     k++);
	set_int_arg(img_is_belly?1:0, 	             k++);

	set_int_arg(navdata->ctrl_state, 	     k++);
	set_int_arg(navdata->vbat_flying_percentage, k++);
	set_float_arg(navdata->theta,                k++);
	set_float_arg(navdata->phi,                  k++);
	set_float_arg(navdata->psi,                  k++);
	set_int_arg(navdata->altitude,    	     k++);
	set_float_arg(navdata->vx,                   k++);
	set_float_arg(navdata->vy,                   k++);
	set_float_arg(navdata->vz,                   k++);

	PyObject * pResult = PyObject_CallObject(pFunc, pArgs);

        if (!pResult) {
		fun_error("Call failed\n", "");
	}

	k = 0;
	commands->start   = get_int_result(pResult,   k++);
	commands->select  = get_int_result(pResult,   k++);
	commands->zap     = get_int_result(pResult,   k++);
	commands->enable  = get_int_result(pResult,   k++);
	commands->phi     = get_float_result(pResult, k++);
	commands->theta   = get_float_result(pResult, k++);
	commands->gaz     = get_float_result(pResult, k++);
	commands->yaw     = get_float_result(pResult, k++);
}

void agent_comm_close() {

	Py_XDECREF(pFunc);
	Py_DECREF(pModule);
	Py_DECREF(pArgs);
	Py_DECREF(pResult);

	Py_Finalize();
}
