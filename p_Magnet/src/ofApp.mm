#include "ofApp.h"
#include "ofxCocoa.h"

static int profileIndex = 0;
int selectIndex = 0;

//ofxSVG arrow;
ofImage arrow;

//--------------------------------------------------------------
ofApp::ofApp( string settings ){
}

//--------------------------------------------------------------
void ofApp::setup(){
    [MSA::ofxCocoa::glWindow() setCollectionBehavior:NSWindowCollectionBehaviorStationary|NSWindowCollectionBehaviorCanJoinAllSpaces|NSWindowCollectionBehaviorFullScreenAuxiliary];
    
    ofBackground( ofColor(0,0,0,0) );
	ofSetFrameRate(60);
    
    string server   = "127.0.0.1";
    string name     = "Gel master";
    
    spacebrew.addSubscribe("touch", "point2d");
    spacebrew.addSubscribe("announce", "announce");
    
    spacebrew.connect( server, name, "");
    ofAddListener(spacebrew.onMessageEvent, this, &ofApp::onMessage);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( auto & i : arrowGrid ) {
        i.draw();
    }
    
    static bool setup = false;
    if ( !setup ){
        setup = true;
        arrow.loadImage("arrow.png");
        
        float xScale = (float) ofGetWidth() / 25.;
        float yScale = (float) ofGetHeight() / 12.;
        
        for (int x =0; x<25; x++){
            for (int y =0; y<12; y++){
                arrowGrid.push_back(ParticleType());
                arrowGrid.back().gridPos.set(x * xScale, y * yScale);
                arrowGrid.back().connect(&arrow);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::onMessage(Spacebrew::Message & m){
    static Json::Reader reader;
    Json::Value root;
    if ( m.name == "announce" ){
        ofStringReplace(m.value, "\\", "");
        bool b = reader.parse( m.value, root);
        if ( root.isObject() && !root.isNull()){
            if ( !root["id"].isNull()){
                string ID = root["id"].asString();
                particles[ID] = &arrowGrid[selectIndex];
                particles[ID]->connect(&arrow);
                particles[ID]->color.r = root["r"].asInt();
                particles[ID]->color.g = root["g"].asInt();
                particles[ID]->color.b = root["b"].asInt();
                selectIndex++;
            }
        } else {
            
        }
    } else if (m.name == "touch"){
        ofStringReplace(m.value, "\\", "");
        bool b = reader.parse( m.value, root);
        if ( b ){
            if ( root.isObject() && root.isMember("id") ){
                string ID = root["id"].asString();
                if ( particles.count(ID) == 0 ){
                    particles[ID] = &arrowGrid[selectIndex];
                    particles[ID]->connect(&arrow);
                    selectIndex++;
                }
                if ( !root["x"].isNull() ){
                    particles[ID]->x = ofToFloat(root["x"].asString()) * ofGetWidth();
                    particles[ID]->y = ofToFloat(root["y"].asString()) * ofGetHeight();
                    
                    
                    cout << getAngle( particles[ID]->gridPos, (*particles[ID])) << endl;
                } else {
                }
                if ( root["color"].isObject()){
                    particles[ID]->color.set(root["color"]["r"].asInt(), root["color"]["g"].asInt(), root["color"]["b"].asInt());
                }
            } else {
            }
        } else {
        }
    } else {
    }
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