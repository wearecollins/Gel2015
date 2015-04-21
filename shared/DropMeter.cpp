//
//  DropMeter.cpp
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#include "DropMeter.h"

//--------------------------------------------------------------
DropMeter::DropMeter(){
    
}

//--------------------------------------------------------------
void DropMeter::setup(){
    
}

//--------------------------------------------------------------
void DropMeter::render(){
    ofPushMatrix();
    
    // cleanup "dead" drops
    for (int i=grid.size()-1; i>=0; i--){
        grid[i]->update(1./60.);
        if ( grid[i]->deadDrop() ){
            delete grid[i];
            grid.erase(grid.begin() + i);
        }
    }

    // check if we're in party mode and add more drops every few frames
    if (bPartyMode && (ofGetFrameNum() % 10) == 0) {
        partyMode();
    }
    
    if (bPartyMode){
        
        // activate based on value
        ofVec2f pnt = getGridPoint( value );

        // add a bit of randomness to the height for now until we re-think the
        // mapped value distribution stuff
        pnt.y += ofRandom(-100, 100);

        // check for nearby drops so we don't draw a mess of drops on top of each other
        float rad = 175;
        bool bFound = false;
        
        for (auto & g : grid ){
            if ( abs( g->distance(pnt)) < rad ){
                bFound = true;
                break;
            }
        }
        
        if ( !bFound && bActive ){
            DropPoint *dp = new DropPoint();
            dp->setup(pnt);
            grid.push_back(dp);
    //        grid.back().activate();
        }
        
        
        for (auto & g : grid ){
            g->draw();
        }
    } else {
        ofSetColor(0);
        ofRect(0, 0, getProjectorWidth(), getProjectorHeight());
        ofSetColor(255);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void DropMeter::partyMode(){
    
    LiveInput::partyMode();

    int dropsToAdd = 4;
    int gridSpacing = 100;

    while (dropsToAdd > 0) {

        // pick a random point from a grid that covers the screen
        int x = (int) ofRandom(0, getProjectorWidth()) / gridSpacing;
        int y = (int) ofRandom(0, getProjectorHeight())  / gridSpacing;
        ofVec2f pnt = ofVec2f(x, y);
        pnt *= gridSpacing;

        // check for nearby circles so we don't draw a mess on top of each other
        float rad = 50;
        bool bFound = false;

        for (auto & g : grid ){
            if ( abs( g->distance(pnt)) < rad ){
                bFound = true;
                break;
            }
        }

        if (bFound) {
            continue;
        } else {
            DropPoint *dp = new DropPoint();
            dp->setup(pnt);
            grid.push_back(dp);
            dropsToAdd--;
        }
    }

}