#  watchForPhotoCommand.sh
#  2012-11-02
#  UdK Digital Class, Short-term project: Freeze

# Kills Apple's PTPCamera process that blocks gphoto2.

# Launches gphoto2 to capture images when user sends
# a "kill -USR1 <PID>" to the gphoto instance we start.

# The Processing Sketch watchForJump.pde is responsible
# for creating the trigger file.

# ----  Setup  -----------------

#!/bin/bash

TRIGGER_FILE="takePhoto.txt"

# Get the gphoto2 pid
GPHOTO_PID=`ps -ax | grep -i [g]photo2 | awk '{ print $1 }'`

# Construct the command used to trigger the photo taking.
TAKE_PHOTO_COMMAND="kill -USR1 ${GPHOTO_PID}"

this=1

# ----  Code ------------------

while [ $this -le 100 ]
do
	if [ -e ${TRIGGER_FILE} ]
	then
		# Get the gphoto2 pid, in case it's changed since we started.
		GPHOTO_PID=`ps -ax | grep -i [g]photo2 | awk '{ print $1 }'`
		if [ "${GPHOTO_PID}" != "" ]
		then
			# Re-construct the trigger command with the most current PID.
			TAKE_PHOTO_COMMAND="kill -USR1 ${GPHOTO_PID}"
			# Optionally delay the time from trigger to shutter.
			# sleep 0.5
			${TAKE_PHOTO_COMMAND}	
			if [ -e ${TRIGGER_FILE} ]
			then
				rm ${TRIGGER_FILE}
			fi
		else
			echo "Can't Take Photo: No gphoto2 process running."
			rm ${TRIGGER_FILE}
		fi
	fi
	sleep 0.1
done
