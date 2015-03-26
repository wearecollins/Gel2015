#include "ofApp.h"

string id;
ofColor color(255);

ofVec2f p,t;

int last = 0;
int rate = 500;

//--------------------------------------------------------------
void ofApp::setup(){
    id = ofToString(ofRandom(10000),0);
    
    spacebrew.addPublish("touch", "gelpoint");
    
    spacebrew.connect( "spacebrew.robotconscience.com", "randomsender_"+ id);
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (ofGetElapsedTimeMillis() - last > rate ){
        rate = ofRandom(5000);
        last = ofGetElapsedTimeMillis();
        
        t.set( ofRandom(-1,1),0);
        cout << "set "<<t<<endl;
    }
    
    p = p * ofVec2f(.7,.7) + t * ofVec2f(.3,.3`);
    
    stringstream ss;
    ss<<"{\"x\":\""<<ofToString(p.x) << "\",";
    ss<<"\"y\":\"" << ofToString(p.y) << "\",";
    ss<<"\"id\":\"" << id << "\",";
        ss<<"\"color\":{";
            ss<<"\"r\":" << (int) (color.r) << ",";
            ss<<"\"g\":" << (int) (color.g) << ",";
            ss<<"\"b\":" << (int) (color.b) << "";
        ss<<"}";
    ss<<"}";
    
    spacebrew.send("touch", "gelpoint", ss.str());
}

//--------------------------------------------------------------
void ofApp::draw(){

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
