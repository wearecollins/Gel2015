#include "ofApp.h"
#include "ofxCocoa.h"

//--------------------------------------------------------------
ofApp::ofApp(){
}

//--------------------------------------------------------------
void ofApp::setup(){
    [MSA::ofxCocoa::glWindow() setCollectionBehavior:NSWindowCollectionBehaviorStationary|NSWindowCollectionBehaviorCanJoinAllSpaces|NSWindowCollectionBehaviorFullScreenAuxiliary];
    
//    [MSA::ofxCocoa::glWindow() setIgnoresMouseEvents:YES];
    
    ofBackground( ofColor(0,0,0,0) );
//	ofSetFrameRate(60);
    
    string server   = "spacebrew.robotconscience.com";
    string name     = "Gel master";
    
    processor.setup(spacebrew);
    gameController.setup(processor,spacebrew);
    
    spacebrew.connect( server, name, "");
    ofAddListener(spacebrew.onMessageEvent, this, &ofApp::onMessage);
    
    ofSetCircleResolution(60);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    
    float renderScale = (float) ofGetWidth() / getProjectorWidth();
    ofScale(renderScale, renderScale);
    gameController.draw();
    
    ofPopMatrix();
    
    static bool bSet = false;
    if (!bSet){
        
//        [MSA::ofxCocoa::glView() setSyncToDisplayLink:YES];
        bSet = true;
    }
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(),3), 20,20);
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
    } else if ( key == '+'){
        gameController.triggerNextFrame();
    } else if ( key == '-'){
        gameController.triggerPrevFrame();
    } else if ( key == 'c'){
        gameController.triggerCelebration();
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