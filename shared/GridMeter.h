//
//  GridMeter.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once

#include "LiveInput.h"
#include "Params.h"

struct Arrow {
    ofVec2f start;
    ofVec2f end;
    ofVec2f otherEnd;
};

class GridPoint : public ofVec2f {
public:
    GridPoint();
    void draw();
    void activate( float level=255);

    float   radius;
    ofColor color;
    
};

class GridMeter : public LiveInput {
    friend class GameController;
public:
    
    GridMeter();
    
    void setup();
    void render();
    void partyMode();

    void editArrows();

    void mouseMoved(ofMouseEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);

protected:

    void setupGrid();
    vector<GridPoint> grid;
    vector<Arrow> arrows;

    bool bWaitingForArrowStartClick;
    bool bWaitingForArrowEndClick;
};
