# Makefile - make file for AR.Drone autopilot agent.
#
#    Copyright (C) 2010 Simon D. Levy
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as 
#    published by the Free Software Foundation, either version 3 of the 
#    License, or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License 
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# You should also have received a copy of the Parrot Parrot AR.Drone 
# Development License and Parrot AR.Drone copyright notice and disclaimer 
# and If not, see 
#   <https://projects.ardrone.org/attachments/277/ParrotLicense.txt> 
# and
#   <https://projects.ardrone.org/attachments/278/ParrotCopyrightAndDisclaimer.txt>.

# Customizable parameters section -------------------------------------------------------------------------------------------------

# SDK version
SDKVER = ARDrone_SDK_Version_1_8_20110726/

# This should be where you put the version above
TOP = ./$(SDKVER)

# This should be where you put ArDroneLib, which you should compile first.  For versions 1.6 and 1.7, make sure to set 
DIR = $(TOP)/ARDroneLib

# Language: comment-out all but desired

# Python version: you may need to run apt-get install python-dev as root
PYVER = 2.7

# If you use Python, make sure that PYTHONPATH shell variable contains . (dot; current directory)
LANGUAGE_AGENT = python_agent
LANGUAGE_LIB = -L/usr/lib/python$(PYVER)/config -lpython$(PYVER)

# If you use Matlab, make sure that MATLAB shell variable points to location of Matlab (e.g. /usr/local/matlabR2010a)
#LANGUAGE_AGENT = matlab_agent
#LANGUAGE_LIB = -L /usr/local/matlabR2010a/bin/glnxa64/ -lmx -leng

#LANGUAGE_AGENT = c_agent

# You can get your gamepad id by issuing the Linux lsusb command

GAMEPAD = GAMEPAD_LOGITECH
GAMEPAD_ID = 0x046dc215

#GAMEPAD = GAMEPAD_PS3
#GAMEPAD_ID = 0x0e8f310d

# Set up any extra libraries you need
#EXTRA_LIB = -lhighgui -lcv -lcxcore

# --------------------------------------------------------------------------------------------------------------------------------
#
# Compiler component of ARDroneLib build path
GCC_VERSION=$(shell $(GENERIC_COMMAND_PREFIX)gcc -v 2>&1 | \
            grep --color=never version | grep -v [cC]onfigur | \
            sed -e "s/\(^version gcc \)\([^ ]*\)\(.*\)/\2/" | \
            sed -e "s/\(^gcc version \)\([^ ]*\)\(.*\)/\2/")

# OS component of ARDroneLib build path
OS_TARGET_ID = $(shell uname -sor | sed -e "s/[ \/]/_/g")

# ARDroneLib build path
SYS = $(OS_TARGET_ID)_gcc_$(GCC_VERSION)

# Useful ARDroneLib subdirectories
SDK = $(DIR)/VP_SDK
SFT = $(DIR)/Soft
TGT = $(SFT)/Build/targets_versions

# Include-path for ARDroneLib headers
INC = -I$(DIR) -I$(SDK) -I$(SDK)/VP_Os/linux -I$(SFT)/Lib -I$(SFT)/Common

# Strict compilation
CC = gcc -Wall

# Project name
ALL = autopilot

# Objects to build and link
OBJS = autopilot.o gamepad.o navdata.o video_stage.o agent.o $(LANGUAGE_AGENT).o 

all: $(ALL)

$(ALL): $(OBJS)
	$(CC) -o $(ALL) $(OBJS) -lpthread -lm \
           -L$(TGT)/ardrone_lib_PROD_MODE_vlib_$(SYS) -lpc_ardrone  \
           -L$(TGT)/sdk_PROD_MODE_vlib_$(SYS) -lsdk \
           -L$(TGT)/vlib_PROD_MODE_$(SYS) -lvlib -lSDL \
	   $(LANGUAGE_LIB) \
	   $(EXTRA_LIB)
           
$(OBJS): Makefile

autopilot.o: autopilot.c autopilot.h agent.h
	$(CC) $(INC) -c autopilot.c

gamepad.o: gamepad.c autopilot.h agent.h
	$(CC) $(INC) -D$(GAMEPAD)_ID=$(GAMEPAD_ID) -c gamepad.c

navdata.o: navdata.c autopilot.h
	$(CC) $(INC) -c navdata.c

video_stage.o: video_stage.c autopilot.h agent.h
	$(CC) $(INC) -c video_stage.c

agent.o: agent.c autopilot.h agent.h agent_comm.h
	$(CC) $(INC) -c agent.c

c_agent.o: c_agent.c agent_comm.h
	$(CC) $(INC) -c c_agent.c

python_agent.o: python_agent.c agent_comm.h
	$(CC) $(INC) -I/usr/include/python$(PYVER) -c python_agent.c

matlab_agent.o: matlab_agent.c agent_comm.h
	$(CC) $(INC) -I $(MATLAB)/extern/include/ -c matlab_agent.c

# Helpful commands

# Run the project
test: $(ALL)
	./autopilot

# See whether the drone is avaialabl
ping:
	ping 192.168.1.1

# Halt the program
kill:
	pkill autopilot

# Test the joystick
js:
	jstest /dev/input/js0

# Remove all but source files
clean: 
	rm -f $(ALL) *.o *.pyc *.txt *~

# Simple version control
backup:
	cp *.c bak
	cp *.h bak
	cp *.py bak
	cp README bak
	cp Makefile bak
