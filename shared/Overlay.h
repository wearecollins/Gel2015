//
//  Overlay.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class Overlay : public ofVec2f {
public:
    
    Overlay(){
        color.a = 0;
        bgColor.set(0,0);
        bActive = false;
    };
    
    virtual void load( string image ){
        overlay.loadImage(image);
        fbo.allocate(ofGetWidth(), ofGetHeight());
    }
    
    virtual void activate( float levelBg=200, float level = 255 ){
        bActive = true;
        targetAlpha     = level;
        targetBgAlpha   = levelBg;
//        color.a = level;
    }
    
    virtual void deactivate(){
        bActive = false;
    }
    
    virtual void draw(){
        if ( !bActive ){
            color.a *= .9;
            bgColor.a *= .9;
        } else {
            color.a = color.a * .9 + targetAlpha * .1;
            bgColor.a = bgColor.a * .9 + targetBgAlpha * .1;
        }
        ofPushStyle();
        ofSetColor(bgColor);
        ofRect(0,0,getProjectorWidth(), getProjectorHeight());
        ofSetColor(color);
        overlay.draw(0,0);
        ofPopStyle();
    }
    
protected:
    
    ofFbo fbo;
    ofImage overlay;
    ofColor color, bgColor;
    float targetAlpha, targetBgAlpha;
    bool bActive;
};
