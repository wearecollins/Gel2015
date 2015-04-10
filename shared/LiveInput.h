//
//  LiveInput.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class LiveInput : public ofVec2f {
public:
    
    LiveInput(){
        messages = NULL;
        fill.a = 0;
        bFadeOut = true;
        bPartyMode = false;
        
        value = 0;
        
        renderTexture.allocate( getProjectorWidth(), getProjectorHeight());//, GL_RGBA );
        
        // this should be centralized, maybe in gamecontroller?
        overlay.loadImage("graphics/overlay.png");
    }
    
    virtual void setup() = 0;
    
    // "live"
    virtual void updateAll( const std::map<string, PointMessage> & messages ){
        this->messages = &messages;
    }
    
    // average
    virtual void update( int val ){
        // easing value, maybe deprecate?
        value = value * .7 + (float) val * .3;
        
        if ( bFadeOut ){
            fill.a *= .9;
        } else {
            fill.a = fill.a * .9 + 255. * .1;
        }
    }
    
    // rename! This says "draw input" or not
    virtual void setActive( bool active = true ){
        bActive = active;
    }
    
    virtual void activate(){
        bFadeOut = false;
        bPartyMode = false;
    }
    
    virtual void deactivate(){
        bFadeOut = true;
    }
    
    void draw(){
        ofSetColor(255);
        overlay.draw(0,0,overlay.width, overlay.height);
        
        render();
    }
    
    virtual void partyMode() { bPartyMode = true; }
    
    virtual void render() = 0;
    
protected:
    ofColor fill;
    float value;
    bool bActive, bFadeOut, bPartyMode;
    ofImage overlay;
    
    ofFbo   renderTexture;
    
    const std::map<string, PointMessage> * messages;
};
