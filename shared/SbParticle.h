//
//  SbParticle.h
//  Arrow
//
//  Created by Brett Renfer on 3/19/15.
//
//

#pragma once

#include "ofMain.h"

class ColoredPoint : public ofVec4f {
public:
    ColoredPoint(){
        this->set(ofVec4f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(.001, .1), ofRandom(.001, .1)));
        color.set( ofRandom(255),ofRandom(255), ofRandom(255));
    }
    ofColor color;
};

class SbParticle : public ofVec3f {
public:
    float radius;
    ofColor color;
    
    SbParticle() :
    radius(3.),
    v(0) {
        for ( int i=0; i<300; i++){
            circles.push_back ( ColoredPoint() );
        }
    }
    
    void draw(){
        ofPushStyle();
        for ( auto & c : circles ){
            c.x = c.x * (1.-c.z) + this->x * c.z;
            c.y = c.y * (1.-c.z) + this->y * c.z;
            c.x += ofSignedNoise( ofGetElapsedTimef() * c.z * 10. + c.z) * 30.;
            c.y += ofSignedNoise( ofGetElapsedTimef() * c.w * 10. + c.w) * 30.;
            ofSetColor(c.color);
            ofCircle(c.x, c.y, radius * ofMap(c.w,.001, .1,.5,1.5) * ofMap(abs(v.x), 0., 1., 1., 1.1, true));
        }
        ofSetColor(color);
        ofPushMatrix();
        ofCircle(*this, radius + fmin(3.,abs(v.x)));
        ofPopMatrix();
        ofPopStyle();
        v = last - *this;
        last.set(*this);
    }
    
    vector<ColoredPoint> circles;
    ofVec3f v, last;
};
