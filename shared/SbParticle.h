//
//  SbParticle.h
//  Arrow
//
//  Created by Brett Renfer on 3/19/15.
//
//

#pragma once

#include "ofMain.h"
#ifdef USE_SVG
#include "ofxSvg.h"
#endif

static float getAngle( ofVec3f vecA, ofVec3f vecB ) {
    return atan2( vecA.x*vecB.y-vecA.y*vecB.x, vecA.x*vecB.x + vecA.y*vecB.y )*RAD_TO_DEG;
}

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
    }
    
    virtual void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofPushMatrix();
        ofCircle(*this, radius);
        ofPopMatrix();
        ofPopStyle();
        v = last - *this;
        last.set(*this);
    }
    
    ofVec3f v, last;
};
#ifdef USE_SVG
class SvgParticle : public SbParticle {
public:
    
    void loadSVG( string path ){
        svg = new ofxSVG();
        svg->load(path);
    }
    
    void connect( ofxSVG * svg ){
        this->svg = svg;
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofPushMatrix();
        ofTranslate(gridPos);
        ofTranslate(svg->getWidth()/2.0, svg->getHeight()/2.0);
        ofRotateZ(getAngle(gridPos, *this));
        ofTranslate(-svg->getWidth()/2.0, -svg->getHeight()/2.0);
        svg->draw();
        ofPopMatrix();
        ofPopStyle();
        v = last - *this;
        last.set(*this);
    }
    
    ofVec3f gridPos;
    
protected:
    ofxSVG * svg;
};

#endif

class ImageParticle : public SbParticle {
public:
    
    ImageParticle() : SbParticle(){
        img = NULL;
    }
    
    void load( string path ){
        img = new ofImage();
        img->loadImage(path);
    }
    
    void connect( ofImage * image ){
        this->img = image;
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofPushMatrix();
        ofTranslate(gridPos);
        if ( img != NULL ){
            ofTranslate(img->getWidth()/2.0, img->getHeight()/2.0);
            ofRotateZ(getAngle(gridPos, *this));
            ofTranslate(-img->getWidth()/2.0, -img->getHeight()/2.0);
            img->draw(0,0);
        }
        ofPopMatrix();
        ofPopStyle();
        v = last - *this;
        last.set(*this);
    }
    
    ofVec3f gridPos;
    
protected:
    ofImage * img;
};
