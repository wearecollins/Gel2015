#include "ofApp.h"
#include "ofxCocoa.h"

static int profileIndex = 0;
int selectIndex = 0;

ofImage arrow;

int sendRate = 100;
int lastSent = 0;

//--------------------------------------------------------------
ofApp::ofApp( string settings ){
}

//--------------------------------------------------------------
void ofApp::setup(){
    [MSA::ofxCocoa::glWindow() setCollectionBehavior:NSWindowCollectionBehaviorStationary|NSWindowCollectionBehaviorCanJoinAllSpaces|NSWindowCollectionBehaviorFullScreenAuxiliary];
    
//    [MSA::ofxCocoa::glWindow() setIgnoresMouseEvents:YES];
//    [MSA::ofxCocoa::glView() setSyncToDisplayLink:YES];
    
    ofBackground( ofColor(0,0,0,0) );
	ofSetFrameRate(60);
    
    string server   = "spacebrew.robotconscience.com";
    string name     = "Gel master";
    
    spacebrew.addSubscribe("announce", "announce");
    spacebrew.addPublish("average", "point2d");
    
    processor.setup(spacebrew);
    gameController.setup(processor,spacebrew);
    
    spacebrew.connect( server, name, "");
    ofAddListener(spacebrew.onMessageEvent, this, &ofApp::onMessage);
    
    ofSetCircleResolution(300);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( ofGetElapsedTimeMillis() - lastSent > sendRate && processor.shouldSend() ){
        lastSent = ofGetElapsedTimeMillis();
        spacebrew.send("average", "point2d", processor.getCurrentJSON());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    
    float renderScale = (float) ofGetWidth() / getProjectorWidth();
    ofScale(renderScale, renderScale);
    gameController.draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::onMessage(Spacebrew::Message & m){
    // these are handled in gamecontroller and inputprocessor
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // debugging
    if ( key == 'l'){
        gameController.triggerLive();
    } else if ( key == 'n'){
        gameController.triggerNextLevel();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}