//
//  DropMeter.cpp
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#include "DropMeter.h"

#pragma mark DropPoint

//--------------------------------------------------------------
DropPoint::DropPoint(){
    
}

//--------------------------------------------------------------
void DropPoint::draw(){
    for ( auto & r : rings ){
        r.x *= 1.01;
        r.y *= 1.01;
        r.z *= .9;
    }
    
    color.a *= .9;
    
    ofPushMatrix();
    ofPushStyle();
    ofFill();
    ofSetColor(color);
    ofCircle(*this, radius);
    
    for ( auto & r : rings ){
        ofNoFill();
        ofCircle(*this, radius * r.x);
    }
    
    //    ofSetColor(255);
    //    ofNoFill();
    //    ofCircle(*this, radius);
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void DropPoint::activate( float level ){
    color.a = level;
    
    rings.clear();
    
    float scale = ofRandom(1.01, 1.2 );
    
    bool bEven = ofRandom(0, 10 ) > 5;
    
    int ran = floor(ofRandom(3,8));
    for (int i=0; i<ran; i++){
        if ( bEven ){
            rings.push_back(ofVec3f( (i + 1 ) * scale,(i + 1 ) * scale, level) );
        } else {
            rings.push_back(ofVec3f( (i + 1 ) * scale,(i + 1 ) * scale, level) );
            scale *= scale;
        }
    }
    
    radius = ofRandom(10,50);
}

#pragma mark DropMeter

//--------------------------------------------------------------
DropMeter::DropMeter(){
    
}

//--------------------------------------------------------------
void DropMeter::setup(){
    
}

//--------------------------------------------------------------
void DropMeter::render(){
    ofPushMatrix();
    
    for (int i=grid.size()-1; i>=0; i--){
        if ( grid[i].color.a == 0.0 ){
            grid.erase(grid.begin() + i);
        }
    }
    
    // activate based on value
    ofVec2f pnt;
    pnt.x = ofMap(value.x, -1., 1.0, 0, getProjectorWidth());
    pnt.y = ofMap(value.y, -1., 1.0, 0, getProjectorHeight());
    float rad = 300;
    
    if ( messages != NULL ){
        
    }
    
    bool bFound = false;
    
    for (auto & g : grid ){
        if ( abs( g.distance(pnt)) < rad ){
            bFound = true;
        }
    }
    
    if ( !bFound && bActive ){
        grid.push_back(DropPoint());
        grid.back().set(pnt);
        grid.back().activate();
    }
    
    
    for (auto & g : grid ){
        g.draw();
    }
    
    ofPopMatrix();
}