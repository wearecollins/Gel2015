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
        bActive = false;
    };
    
    virtual void load( string image ){
        overlay.loadImage(image);
        fbo.allocate(ofGetWidth(), ofGetHeight());
    }
    
    virtual void activate( float level = 255 ){
        bActive = true;
        color.a = level;
    }
    
    virtual void deactivate(){
        bActive = false;
    }
    
    virtual void draw(){
        if ( !bActive ){
            color.a *= .9;
        }
        ofPushStyle();
        ofSetColor(0, color.a);
        ofRect(0,0,getProjectorWidth(), getProjectorHeight());
        ofSetColor(color);
        overlay.draw(0,0);
        ofPopStyle();
    }
    
protected:
    
    ofFbo fbo;
    ofImage overlay;
    ofColor color;
    bool bActive;
};
