# Gel2015

# Setup
``` 
	cd openframeworks/apps/
	git clone https://github.com/wearecollins/Gel2015.git
	cd Gel2015
	bash cloneaddons.sh
```

# Running
* Run spacebrew persistent admin, pointed to spacebrew.robotconscience.com
	* Make sure the following persistent routes exist:
		* 	``` 
				.*,touch,.*,touch

				* .*,announce,.*,announce
				
				* .*,average,.*,average
			```
* Open Skype and call somebody
* Run GelOuput.app
* Run Mobile output and input on separate devices
	* To-be-added to gh-pages; for now, run via local server
* Be amazed

# Structure

## GelOutput
* Main output app
* Transparent window that is meant to run on top of Skype
	* NOTE: It's an OS X Agent, which means it does NOT have an icon, so needs to be killed via command line (or via Xcode when debugging)
	* Mouse events pass through by default, so can interact with OS otherwise like normal
* Many main classes in shared/*

## GelController
* To be created
* App from which conductor (Brett) will trigger events, e.g. "Start Level One", "Level One Complete"

## p_.*
* OF output prototypes and tests

## prototypes/mobile_output
* Arrow that tells avatar to walk forward, turn, or do nothing

## prototypes/mobile_input
* Interafce for audience to control Mr. Avatar
