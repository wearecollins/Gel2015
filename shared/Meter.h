//
//  Meter.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once
#include "LiveInput.h"

#include "ofxSvg.h"
#include "ofxAnimatableFloat.h"

// Holds 1/3 of the Raibow (left/middle/right)
struct RainbowSegment {
    ofMesh fullMesh;                        // the full line
    vector<ofMesh> meshes;                  // "radiating" lines
    vector <ofxAnimatableFloat*> pulses;    // ~0->1 pulse through the segment
    Poco::LocalDateTime lastPulse;
};

// Holds shorter segments within each RainbowSegment for party mode
struct PartySegment {
    int startingIndex = 0;
    ofColor color;
};

class Meter : public LiveInput {
public:
    
    Meter() : LiveInput(){
        setup();
    }
    
    void setup();
    void render();
    void partyMode();

protected:
    
    ofImage meter;
    ofShader renderShader;
    ofxSVG svg;

    // helpers & callbacks
    ofMesh createMesh(ofPolyline& line, int lowerBound, int higherBound, ofColor color);
    float calcAlphaForPulse(float percent, int index, int numLines, float overlap);
    void createPulse(float value);
    void drawClosestPoint();
    void onAnimFinished(ofxAnimatable::AnimationEvent& args);

    Poco::LocalDateTime lastPulse;
    vector<ofPolyline> lines;
    vector<ofColor> colors;
    vector<RainbowSegment> segments;
    vector<PartySegment> partyModeSegments;

};
