#ifndef _TEST_APP
#define _TEST_APP

/****************************************************
	OF + ADDONS
****************************************************/

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxXmlSettings.h"

#include "ofxSpacebrew.h"

#include "Constants.h"
#include "InputProcessor.h"
#include "GameController.h"

/****************************************************
	ofApp
****************************************************/

class ofApp : public ofBaseApp{

public:    
    ofApp();
    void exit();
    
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    string type;
    
    vector<Spacebrew::Connection *> spacebrew;
    
    void onMessage( Spacebrew::Message & m );
    
    GameController  gameController;
    InputProcessor  processor;
};

#endif
