#include "ofApp.h"
#include "ofxCocoa.h"

//--------------------------------------------------------------
ofApp::ofApp(){
}

//--------------------------------------------------------------
void ofApp::setup(){
    [MSA::ofxCocoa::glWindow() setCollectionBehavior:NSWindowCollectionBehaviorStationary|NSWindowCollectionBehaviorCanJoinAllSpaces|NSWindowCollectionBehaviorFullScreenAuxiliary];
    
    [MSA::ofxCocoa::glWindow() setIgnoresMouseEvents:YES];
    
    ofBackground( ofColor(0,0,0,0) );
//	ofSetFrameRate(60);
    
    string server   = "spacebrew.robotconscience.com";
    string name     = "Gel master";
    
    processor.setup(spacebrew);
    gameController.setup(processor,spacebrew);
    
    spacebrew.setAutoReconnect();
    spacebrew.connect( server, name, "");
    spacebrew.addSubscribe("windowEvent", "string");
    
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
    
    if (m.name == "windowEvent"){
        if (m.value == "level_prev"){
            gameController.triggerPrevLevel();
            
        } else if (m.value == "level_next"){
            gameController.triggerNextLevel();
        } else if (m.value == "slide_prev"){
            gameController.triggerPrevFrame();
        } else if (m.value == "slide_next"){
            gameController.triggerNextFrame();
            
        } else if (m.value == "start_level"){
            gameController.triggerLive();
        } else if (m.value == "finish_level"){
            gameController.triggerCelebration();
            
        } else if (m.value == "mouse_on"){
            [MSA::ofxCocoa::glWindow() setIgnoresMouseEvents:NO];
        } else if (m.value == "mouse_off"){
            [MSA::ofxCocoa::glWindow() setIgnoresMouseEvents:YES];
        }
    }
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
    } else if ( key == 'h'){
        gameController.guiToggleVisible();
    }  else if ( key == '0'){
        gameController.guiToggleMinified();
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