#!/bin/bash

trap "kill 0" SIGINT

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js --host spacebrew.robotconscience.com --port 9000 -s 9010 &)
sleep 1

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js --host spacebrew.robotconscience.com --port 9001 -s 9011 &)
sleep 1

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js --host spacebrew.robotconscience.com --port 9002 -s 9012 &)
sleep 1

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js --host spacebrew.robotconscience.com --port 9003 -s 9013 &)
#sleep 1
#(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/DirectionsOutput/bin; ./DirectionsOutput.app/Contents/MacOS/DirectionsOutput &)
sleep 1
(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/Averager; node app.js --host spacebrew.robotconscience.com )

#wait
