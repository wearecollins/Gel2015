//
//  Meter.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once
#include "LiveInput.h"
#include "Params.h"

#include "ofxSvg.h"
#include "Svg2Mesh.h"
#include "ofxAnimatableFloat.h"

// animated point in mesh
struct MeshBlip {
    int vertexIndex;
    ofxAnimatableFloat * animation;
    ofMesh * parent;
};

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
    ofxSVG svg;

    // helpers & callbacks
    ofMesh createMesh(ofPolyline& line, int lowerBound, int higherBound, ofColor color);
    float calcAlphaForPulse(float percent, int index, int numLines, float overlap);
    void createPulse(float value);
    void createBlip(string name, float value);
    void drawClosestPoint();
    void onAnimFinished(ofxAnimatable::AnimationEvent& args);
    void onBlipFinished(ofxAnimatable::AnimationEvent& args);

    Poco::LocalDateTime lastPulse;  // not actually used!
    vector<ofPolyline> lines;       // only used to create meshes & in drawClosestPoint()
    vector<RainbowSegment> segments;
    vector<PartySegment> partyModeSegments;

    // 'live view' meshes (L,U,R)
    vector<vector<ofMesh> > liveViewMeshes;
    std::map<string, MeshBlip* > blips;
    
};
