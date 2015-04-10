#include "ofMain.h"
#include "ofApp.h"

#include "ofxCocoa.h"
#include <Cocoa/Cocoa.h>

//========================================================================
int main( int argc, char * const argv[] )
{
    
	// SETUP COCOA WINDOW
	
	MSA::ofxCocoa::InitSettings			initSettings;
	initSettings.isOpaque				= false;
	initSettings.windowLevel			= NSScreenSaverWindowLevel;
	initSettings.hasWindowShadow		= false;
//	initSettings.numFSAASamples			= 4;
    
    // change these vars to adjust the width and height
	initSettings.initRect.size.width	= MSA::ofxCocoa::rectForAllScreens().size.width;
	initSettings.initRect.size.height	= MSA::ofxCocoa::rectForAllScreens().size.height;
    
    //leave these unless you don't want it centered
    // might have to change it to something other than rectForAllScreens() if there is > 1 screen (e.g. with a KVM?)
    initSettings.initRect.origin.x      = MSA::ofxCocoa::rectForAllScreens().size.width / 2.0f - initSettings.initRect.size.width/2.0f;
    initSettings.initRect.origin.y      = MSA::ofxCocoa::rectForAllScreens().size.height / 2.0f - initSettings.initRect.size.height/2.0f;
	initSettings.windowMode				= OF_WINDOW;
	initSettings.windowStyle			= NSBorderlessWindowMask;
	initSettings.initRect				= MSA::ofxCocoa::rectForAllScreens();
//    initSettings.initRect.origin.y      -= 100;
	
	MSA::ofxCocoa::AppWindow		cocoaWindow(initSettings);
    
	ofSetupOpenGL(&cocoaWindow, 0, 0, 0);		// all other parameters are ignored, use initSettings above
	
    // START TEST APP
	
	ofApp* app = new ofApp();
	ofRunApp( app );
}
