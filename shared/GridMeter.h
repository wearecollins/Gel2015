//
//  GridMeter.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once

#include "LiveInput.h"

class GridPoint : public ofVec2f {
public:
    GridPoint();
    void draw();
    void activate( float level=255);

    float   radius;
    ofColor color;
    
};

class GridMeter : public LiveInput {
public:
    
    GridMeter();
    
    void setup();
    void render();
    
protected:
    vector<GridPoint> grid;
};
