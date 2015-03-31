//
//  DropMeter.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once 

#include "LiveInput.h"

class DropPoint : public ofVec2f {
public:
    DropPoint();
    void draw();
    void activate( float level=255);
    
    float   radius;
    ofColor color;
    
protected:
    vector<ofVec3f> rings;
};


class DropMeter : public LiveInput {
public:
    
    DropMeter();
    
    void setup();
    void render();
    
protected:
    vector<DropPoint> grid;
};