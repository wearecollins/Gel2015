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
#include "ofxAnimatableFloat.h"

// This is an arrow
// Am I crazy or brilliant?
// --Both? - BR

/*
           ____B
          /|
         / |
        /  |
       /  /---C
    A /  /____D
      \  \
       \  \___E
        \  |
         \ |
          \|
           ---F

     A = tip
 B & F = outer endpoints
 C & E = inner endpoints
     D = inner point
 
 */

class Arrow {
    friend class GridMeter;
public:
    void calcInnerEndPoints();
    void calcInnerPoint();
    
    Poco::LocalDateTime lastPulse;

protected:
    ofPoint pointA;
    ofPoint pointB;
    ofPoint pointC;
    ofPoint pointD;
    ofPoint pointE;
    ofPoint pointF;
    ofPolyline shape;
};

struct Pulse {
    // makes me crazy that these have to be pointers :(
    ofxAnimatableFloat * anim;
    ofRectangle shape;
    
    int dir; //0-2
};

class GridPoint : public ofVec2f {
public:
    GridPoint();
    void draw();
    void activate( float level=255, bool force = false);

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
    void calcArrowOuterEndpoints(ofVec2f pos);
    void calcArrowInnerEndpoints();
    void calcArrowThickness();

    void mouseMoved(ofMouseEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);
    void mouseReleased(ofMouseEventArgs& args);

protected:

    void setupGrid();
    vector<GridPoint> grid;
    vector<Arrow> arrows;
    vector<Pulse> pulses;

    float pulseRateSeconds;
    
    bool bWaitingForArrowStartClick;
    bool bWaitingForArrowEndClick;
};
