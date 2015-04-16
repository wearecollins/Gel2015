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
    
//    ofSetColor(255);
//    ofNoFill();
//    ofCircle(*this, radius);
    
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
    renderTexture.begin();
    ofClear(255);
    ofPushMatrix();
    
    // activate based on value
    ofVec2f pnt = getGridPoint(value);
    
    // radius of "hit" based on grid point
    float rad = 300;
    
    if ( messages != NULL ){
        for (auto & g : grid ){
            for ( auto & m : *messages ){
                ofVec2f pnt2 = getGridPoint(m.direction);
                if ( abs( g.distance(pnt2)) < rad/4. && bActive ){
                    g.activate(50);
                }
            }
        }
    }
    
    static bool blogged = false;
    
    for (auto & g : grid ){
        if ( bActive && !blogged ){
//            cout << abs( g.distance(pnt)) << endl;
        }
        if ( abs( g.distance(pnt)) < rad && bActive ){
            g.activate();
        }
        g.draw();
    }
    
    if ( !blogged && bActive){
        blogged = true;
    }
    
    ofPopMatrix();
    renderTexture.end();
    
    ofSetColor(255, fill.a);
    
    renderTexture.draw(0,0);
}

//--------------------------------------------------------------
void GridMeter::partyMode(){
    
}
