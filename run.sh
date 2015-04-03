#!/bin/bash

#COUNTER=0
#TOTAL=2
#
#while [  $COUNTER -lt $TOTAL ]; do
#
#osascript &>/dev/null <<EOF
#tell application "Terminal" to activate
#tell application "Terminal" to do script "cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/p_RandomSender/bin; p_RandomSenderDebug.app/Contents/MacOS/p_RandomSenderDebug"
#EOF
#
#sleep 1
#
#
#let COUNTER=COUNTER+1

trap "kill 0" SIGINT

(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_server_forever.js &)
sleep 1
(cd ~/Dropbox/Code/spacebrew/spacebrew; node node_persistent_admin.js &)
sleep 1
(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/Averager; node app.js)

#wait
