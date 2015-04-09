//
//  DropPoint.h
//  DropPoint
//
//  Created by Matt Felsen on 3/4/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class DropPoint : public ofVec2f {

public:

    void setup(ofVec2f pos);
    void setup(float x, float y);
    void update(float dt);
    void draw();

    bool deadDrop();

protected:

    float centerSize;
    float ringTargetSize;
    float rate;
    float numCircles;

    ofColor ringColor;
    ofColor centerColor;

    vector<ofxAnimatableFloat> animSize;
    vector<ofxAnimatableFloat> animOpacity;

};