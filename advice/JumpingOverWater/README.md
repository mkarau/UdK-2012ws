Jumping Over Water
UdK-2012ws
Digital Class, Short-term Project: Freeze
==========

Concept: Jump over water and you photo is taken mid-air

1. Arduino with E18-D80NK sensor detects "Jump" and sends serial text "Jump".
2. Processing sketch "watchForJump.pde" listens for "Jump" and touches a text file when a jump happens.
3. "watchForPhotoCommand.sh" watches for the text file created by processing.  When the file exists, it triggers gphoto2 to take an image and this script deletes the trigger file.
4. "jumpOverWater.sh" kills the PTPCamera program from Apple that blocks gphoto2 usage and starts a gphoto2 process that listens for SIGINT commands via the kill program.  "kill -USR1 <gphoto_pid>" triggers a shutter release.

