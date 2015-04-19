#include "ofApp.h"

string id;
ofColor color(255);

ofVec2f p,t;

int last = 0;
int rate = 500;

int number = 300;

//--------------------------------------------------------------
void ofApp::setup(){
    times.resize(number);
    values.resize(number);
    
    for ( int i=0; i<number; i++){
        spacebrew.push_back(new Spacebrew::Connection());
        id = ofToString(ofRandom(10000),0);
        
        values[i] = (ofRandom(2));
        times[i] = ofRandom(10000);
        
        spacebrew.back()->addPublish("direction", "pad");
        spacebrew.back()->connect( "127.0.0.1", "randomsender_"+ id);
    }
    
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    float x =0;
    float y = 20;
    float w = ofGetWidth() / 15.;
    float h = (ofGetHeight()-20) / 20.;
    
    for (int i=0; i<times.size(); i++){
        if ( ofGetElapsedTimeMillis() - times[i] > rate){
            times[i] = ofGetElapsedTimeMillis();
            values[i] = round(ofNoise( x + ofGetElapsedTimef())* 2 );
            
            spacebrew[i]->send("direction", "pad", "\""+ofToString(values[i]) +":"+spacebrew[i]->getConfig()->clientName+":0\"");
        }
        
        x += w;
        if ( x + w > ofGetWidth() ){
            x = 0;
            y += h;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    float x =0;
    float y = 20;
    float w = ofGetWidth() / 15.;
    float h = (ofGetHeight()-20) / 20.;
    ofNoFill();
    
    float t = 0;
    
    for ( auto & v : values ){
        ofRect(x, y, w, h);
        ofDrawBitmapString(ofToString(v), ofPoint(x + 20, y + 20));
        x += w;
        if ( x + w > ofGetWidth() ){
            x = 0;
            y += h;
        }
        
        t += v;
    }
    
    t /= values.size();
    
    ofDrawBitmapString("AVERAGE :"+ofToString(t), 0,20);
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}