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
        
        renderTexture.allocate( getProjectorWidth(), getProjectorHeight());//, GL_RGBA );
    }
    
    virtual void setup() = 0;
    
    
    virtual void updateAll( const std::map<string, PointMessage> & messages ){
        this->messages = &messages;
    }
    
    virtual void update( ofVec2f val ){
        value = value * ofVec2f(.7,.7) + val * ofVec2f(.3,.3);
        
        // this should be centralized, maybe in gamecontroller?
        if ( !overlay.isAllocated() ){
            overlay.loadImage("graphics/overlay.png");
        }
        
        if ( bFadeOut ){
            fill.a *= .9;
        } else {
            fill.a = fill.a * .9 + 255. * .1;
        }
    }
    
    virtual void setActive( bool active = true ){
        bActive = active;
    }
    
    virtual void activate(){
        bFadeOut = false;
    }
    
    virtual void deactivate(){
        bFadeOut = true;
    }
    
    void draw(){
        ofSetColor(255);
        overlay.draw(0,0,overlay.width, overlay.height);
        
        render();
    }
    
    virtual void render() = 0;
    
protected:
    ofColor fill;
    ofVec2f value;
    bool bActive, bFadeOut;
    ofImage overlay;
    
    ofFbo   renderTexture;
    
    const std::map<string, PointMessage> * messages;
};
