#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetWindowPosition(0, 0);
    ofSetWindowShape(1920, 1080);
    ofBackground(ofColor::white);
//    dot.setup(ofRandomWidth(), ofRandomHeight());

    img.loadImage("../../../reference_image.png");

}

//--------------------------------------------------------------
void ofApp::makeDropPoint(float x, float y){
    DropPoint* drop = new DropPoint();
    drop->setup(x, y);
    drops.push_back(drop);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = drops.size()-1; i >= 0; i--){
        drops[i]->update(1.0/60.0);

        if (drops[i]->deadDrop()){
            drops.erase(drops.begin() + i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(ofColor::white);
    img.draw(0, 0, 1920, 1080);

    for (auto& drop : drops) drop->draw();
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
    makeDropPoint(x, y);
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
