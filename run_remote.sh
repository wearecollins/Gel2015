#!/bin/bash

trap "kill 0" SIGINT

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js --host spacebrew.robotconscience.com &)
sleep 1
(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/Averager; node app.js --host spacebrew.robotconscience.com )

#wait
