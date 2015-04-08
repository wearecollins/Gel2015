# Gel2015

# Setup
``` 
	cd openframeworks/apps/
	git clone https://github.com/wearecollins/Gel2015.git
	cd Gel2015
	bash cloneaddons.sh
```
* Note: if you already have ofxCocoa (or some version of it), do make sure you're on my dev branch
	* See cloneaddons.sh if curious

# Running
* Run a local spacebrew server
* Run spacebrew persistent admin, pointed to your local server
	* Make sure the following persistent routes exist:
		* 	``` 
				.*,direction,.*,touch
				
				* .*,average,.*,average
			```
* Run the Average app:
	* 	```
		cd /Wherever/Gel2015/
		cd Averager
		node app.js
		```
* Run the Dictation app:
	* Gel2015/DirectionsOutput
		* For this to work you MUST have some addressable audio device in your System Preferences / Sound
		* By default it addresses Voila (this is set in Gel2015/DirectionsOutput/bin/data/settings.xml <device>)

* Open Skype and call somebody
	* Make sure the same device Dictation is addressing is set to the mic in Skypte

* Run GelOuput.app

* Run Mobile input 
	* Make sure your Mobile is on the same network as output
	* Navigate to http://bit.ly/gel15i_proto

* Be amazed
	* If Dictation is set up, you should hear commands via Skype at the end point. It's weird.

# Structure

## GelOutput
* Main output app
* Transparent window that is meant to run on top of Skype
	* NOTE: It's an OS X Agent, which means it does NOT have an icon
		* To kill or use keyboard, you should click on the window to give focus, then you're good as normal
	* NOT TRUE BUT WILL BE EVENTUALLY: Mouse events pass through by default, so can interact with OS otherwise like normal
* Many main classes in shared/*

# Averager
	* Node app that takes in input, averages it, then barfs it over to the GelOutput

# Gel Voice
	* To be created
	* Uses ofxSpeech and some sort of hack to send via Skype microphone
		* Based on: https://github.com/robotconscience/spacebrew-noodles/tree/master/openFrameworks/spacebrew-speech

## prototypes/mobile_input
* Interafce for audience to control Mr. Avatar

## prototypes/mobile_output
* Arrow that tells avatar to walk forward, turn, or do nothing
* DEPRECATED!

## p_.*
* OF output prototypes and tests
