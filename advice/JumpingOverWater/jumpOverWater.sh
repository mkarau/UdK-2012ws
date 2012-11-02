#  JumpingOverWater.sh
#  2012-11-02
#  Code: Matt Karau (mkarau@gmail.com)
#  UdK Digital Class, Short-term project: Freeze

# Kills Apple's PTPCamera process that blocks gphoto2.

# Launches gphoto2 to capture images when user sends
# a "kill -USR1 <PID>" to the gphoto instance we start.

# watchForPhotoCommand.sh is responsible for triggering
# photos.

# ----  Setup  -----------------

#!/bin/bash

KILL_PTP_COMMAND="killall PTPCamera"

# This gphoto filename format archives all the photos with unique names.
#GPHOTO_FILENAME="jumpPhotos/%Y/%m/%d/%Y-%m-%d---%H.%M.%S.%C"

# This gphoto filename format saves every image to the same
# filename, over-writting previous images.
GPHOTO_FILENAME="jumpPhotos/wallpaper.%C"

START_GPHOTO2_COMMAND="gphoto2 --capture-image-and-download --force-overwrite --filename ${GPHOTO_FILENAME} -F 100000 -I -1"

# ----  Code ------------------

echo "Stopping Apple PTPCamera"
${KILL_PTP_COMMAND}


echo "Starting gphoto capture"
${START_GPHOTO2_COMMAND}
