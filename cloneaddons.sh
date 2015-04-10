#!/bin/bash

#make sure you are in the Gel folder when you run this.

cd ../../addons


PREFIX="git clone https://github.com/"

# gui
${PREFIX}rezaali/ofxUI.git

# comms
${PREFIX}labatrockwell/ofxLibwebsockets.git
${PREFIX}Spacebrew/ofxSpacebrew.git

# ui
${PREFIX}robotconscience/ofxCocoa.git --branch dev

# speech
${PREFIX}latrokles/ofxSpeech.git

# animation
${PREFIX}armadillu/ofxAnimatable.git