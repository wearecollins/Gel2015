#include "ofApp.h"
#include "ofxCocoa.h"

static int profileIndex = 0;

ofMesh m;

//--------------------------------------------------------------
ofApp::ofApp( string settings ){
}

//--------------------------------------------------------------
void ofApp::setup(){
    [MSA::ofxCocoa::glWindow() setCollectionBehavior:NSWindowCollectionBehaviorStationary|NSWindowCollectionBehaviorCanJoinAllSpaces|NSWindowCollectionBehaviorFullScreenAuxiliary];
    
    ofBoxPrimitive arrow;
    arrow.set(100., 100., 200);
    
    m.addVertices( arrow.getMesh().getVertices() );
    m.addIndices( arrow.getMesh().getIndices() );
    
    
    ofBackground( ofColor(0,0,0,0) );
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

ofLight light;

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    
    light.enable();
    light.setPosition(ofGetWidth()/2.0, ofGetHeight()/2.0, 100);
    ofTranslate(mouseX, mouseY);
//    arrow.getMesh().draw();
//    ofRect(mouseX,mouseY,50,50);
    
    ofVec2f dir(mouseX,mouseY);
    ofVec2f c(ofGetWidth()/2.0, ofGetHeight()/2.0);
    
    ofRotateZ(ofGetElapsedTimeMillis() * .01);
    ofRotateX(ofGetElapsedTimeMillis() * .001);
    ofRotateY(ofGetElapsedTimeMillis() * .05);
    m.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}