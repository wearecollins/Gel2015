#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxAnimatableFloat.h"

struct RainbowSegment {
    ofPolyline line;
    vector<ofMesh> meshes;
    vector <ofxAnimatableFloat*> pulses;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

protected:

    // helpers & callbacks
    ofMesh createMesh(ofPolyline& line, int lowerBound, int higherBound);
    float calcAlphaForPulse(float percent, int index, int numLines, float overlap);
    void createPulse(float value);
    void drawClosestPoint();
    void onAnimFinished(ofxAnimatable::AnimationEvent& args);

    ofImage mask;
    ofxSVG svg;
    ofColor c;

    vector <ofPolyline> lines;
    vector <RainbowSegment> segments;

};
