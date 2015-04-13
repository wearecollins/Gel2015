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

struct RainbowSegment {
    ofPolyline line;
    vector<ofMesh> meshes;
    vector <ofxAnimatableFloat*> pulses;
    Poco::LocalDateTime lastPulse;
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
    vector <ofPolyline> lines;
    vector <RainbowSegment> segments;

};
