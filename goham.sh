#!/bin/bash

COUNTER=0
TOTAL=300

trap "kill 0" SIGINT

while [  $COUNTER -lt $TOTAL ]; do

(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/Tester; node app.js --host spacebrew.robotconscience.com &)

sleep .1

let COUNTER=COUNTER+1

done

(cd /Users/brettrenfer/Dropbox/Code/OF/of_v0.8.4_osx_release/apps/Gel2015/Tester; node app.js --host spacebrew.robotconscience.com)