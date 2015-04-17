#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"
#include "ofxSpeech.h"

#include "InputProcessor.h"

struct RateLimiter {
    int sendRate; // how often to send, in millis
    int lastSent;     // last sent, in millis
};

class ofApp : public ofBaseApp{

	public:
    
        enum Direction {
            DIRECTION_LEFT = 0,
            DIRECTION_STRAIGHT,
            DIRECTION_RIGHT,
//            DIRECTION_LOOK,
            DIRECTION_STRAIGHT_FAST,
            DIRECTION_STRAIGHT_FASTER,
            DIRECTION_STOP
        };
    
		void setup();
		void update();
		void draw();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        Spacebrew::Connection spacebrew;
        void onMessage( Spacebrew::Message & m );
    
        ofxSpeechSynthesizer speech;
    
        void speak( int direction );
        map<Direction, RateLimiter> rates;
    
        bool bWalking;
    
        // process stuff from spacebrew
        InputProcessor inputProcessor;
    
        // ignore updates
        int currentLevel;
};
