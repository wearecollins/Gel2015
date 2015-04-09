//
//  DropPoint.cpp
//  DropPoint
//
//  Created by Matt Felsen on 3/4/15.
//
//

#include "DropPoint.h"

void DropPoint::setup(ofVec2f pos){
    setup(pos.x, pos.y);
}

void DropPoint::setup(float x, float y){

    this->set(x, y);

    numCircles = 4;
    centerSize = 20;
    ringTargetSize = 200;
    rate = 1.3;

    // random color for center circle & rings
    float ringRand = ofRandom(1);
    float centerRand = ofRandom(1);

    if (ringRand < 0.33)
        ringColor = ofColor::cyan;
    else if (ringRand < 0.66)
        ringColor = ofColor::magenta;
    else
        ringColor = ofColor::yellow;

    if (centerRand < 0.33)
        centerColor = ofColor::cyan;
    else if (ringRand < 0.66)
        centerColor = ofColor::magenta;
    else
        centerColor = ofColor::yellow;

    animSize.clear();
    animOpacity.clear();

    // set up animatables for size & opacity of rings
    for (int i = 0; i < numCircles; i++) {
        ofxAnimatableFloat size, opacity;

        size.reset(0.1);
        size.setDuration(rate);
        size.setCurve(LINEAR);
        size.animateToAfterDelay(1, i / numCircles);

        opacity.reset(1);
        opacity.setDuration(rate);
        opacity.setCurve(EASE_OUT);
        opacity.animateToAfterDelay(0, i / numCircles);

        animSize.push_back(size);
        animOpacity.push_back(opacity);
    }

}

bool DropPoint::deadDrop(){
    // dead if the last ring's alpha is < 0
    return (animOpacity[numCircles-1].val() <= 0);
}

void DropPoint::update(float dt){
    for (auto& anim : animSize) anim.update(dt);
    for (auto& anim : animOpacity) anim.update(dt);
}

void DropPoint::draw(){

    ofNoFill();
    ofSetCircleResolution(60);

    // Draw the rings
    for (int i = 0; i < animSize.size(); i++) {
        ofSetLineWidth(4 * animOpacity[i].val());
        ofSetColor(ringColor, animOpacity[i].val() * 255);
        ofCircle(*this, animSize[i].val() * ringTargetSize);
    }

    ofFill();

    // draw an expanding center circle with lower alpha
    ofSetColor(centerColor, animOpacity[0].val() * 127);
    ofCircle(*this, animSize[0].val() / 2 * ringTargetSize);

    // draw the center with the opacity from the last ring
    ofSetColor(centerColor, animOpacity[numCircles-1].val() * 255);
	ofCircle(*this, centerSize);

}