//
//  GridMeter.cpp
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#include "GridMeter.h"

#pragma mark GridPoint

//--------------------------------------------------------------
GridPoint::GridPoint(){
    radius = 50;
    color.set(255, 0);
}

//--------------------------------------------------------------
void GridPoint::draw(){
    color.a *= .9;
    
    ofPushMatrix();
    ofPushStyle();
    ofFill();
    ofSetColor(color);
    ofCircle(*this, radius);
    
    ofSetColor(255);
    ofNoFill();
    ofCircle(*this, radius);
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void GridPoint::activate( float level ){
    color.a = level;
}


#pragma mark GridMeter

//--------------------------------------------------------------
GridMeter::GridMeter()  : LiveInput(){
    setup();
}

//--------------------------------------------------------------
void GridMeter::setup(){
    // grid of 300?
    float space = 10;
    float w = getProjectorWidth() - space * 5.;
    float h = getProjectorHeight() - space * 5.;
    
    float x_spacing = (float) w / 25.;
    float y_spacing = (float) h / 12.;
    
    float rad = min( x_spacing - space, y_spacing - space ) / 2.0;
    
    for (int x=0; x<25.; x++){
        for (int y=0; y<12.; y++){
            grid.push_back(GridPoint());
            grid.back().set( rad + x * x_spacing, rad + y * y_spacing );
            grid.back().radius = rad;
            grid.back().color.setSaturation(255);
            grid.back().color.setBrightness(255);
            grid.back().color.setHue(ofMap(x, 0, 25., 0, 125) + ofMap(y, 0, 12., 0, 130));
        }
    }
}

//--------------------------------------------------------------
void GridMeter::render(){
    ofPushMatrix();
    float renderScale = (float) ofGetWidth() / getProjectorWidth();
    ofScale(renderScale, renderScale);
    
    // activate based on value
    ofVec2f pnt;
    pnt.x = ofMap(value.x, -1., 1.0, 0, ofGetWidth());
    pnt.y = ofMap(value.y, -1., 1.0, 0, ofGetHeight());
    float rad = 100;
    
    if ( messages != NULL ){
        for (auto & g : grid ){
            for ( auto & m : *messages ){
                ofVec2f pnt2;
                pnt2.x = ofMap(m.second.point.x, -1., 1.0, 0, ofGetWidth());
                pnt2.y = ofMap(m.second.point.y, -1., 1.0, 0, ofGetHeight());
                if ( abs( g.distance(pnt2)) < rad/4. && bActive ){
                    g.activate(50);
                }
            }
        }
    }
    
    for (auto & g : grid ){
        if ( abs( g.distance(pnt)) < rad && bActive ){
            g.activate();
        }
        g.draw();
    }
    ofPopMatrix();
}
