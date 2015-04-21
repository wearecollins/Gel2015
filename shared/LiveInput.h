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
    friend class GameController;
public:
    
    LiveInput(){
        fill.a = 0;
        bFadeOut = true;
        bPartyMode = false;
        
        GEL_COLORS.push_back(ofColor::yellow);
        GEL_COLORS.push_back(ofColor::cyan);
        GEL_COLORS.push_back(ofColor::magenta);
        
        value = 0;
        
        renderTexture.allocate( getProjectorWidth(), getProjectorHeight());//, GL_RGBA );
        
        // this should be centralized, maybe in gamecontroller?
        overlay.loadImage("graphics/overlay.png");
    }
    
    virtual void setup() = 0;
    
    // "live"
    virtual void updateAll( vector<PointMessage> & messages_in ){
        messages = &messages_in;
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
        bPartyMode = false;
    }
    
    void draw(){
        ofSetColor(255);
        overlay.draw(0,0,overlay.width, overlay.height);
        
        render();
    }
    
    virtual void partyMode() { bPartyMode = true; }
    
    virtual void render() = 0;
    
protected:
    // the holy gel colors
    vector<ofColor> GEL_COLORS;
    
    ofColor fill;
    float value;
    bool bActive, bFadeOut, bPartyMode;
    ofImage overlay;
    
    ofFbo   renderTexture;
    
    vector<PointMessage> * messages;
};
