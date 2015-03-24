#!/bin/bash

screen -DR spacebrew
screen -X spacebrew
cd ~/Dropbox/Code/spacebrew/spacebrew
node node_server_forever.js
screen -dS spacebrew

screen -DR spacebrew_persist
screen -X spacebrew
cd ~/Dropbox/Code/spacebrew/spacebrew
node node_persistent_admin.js
screen -dS spacebrew_persist