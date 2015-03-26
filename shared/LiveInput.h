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
    }
    
    virtual void setActive( bool active = true ){
        bActive = active;
    }
    
    void draw(){
        ofSetColor(255);
        
        float scale = (float) ofGetWidth() / overlay.getWidth();
        overlay.draw(0,0,overlay.width * scale, overlay.height * scale);
        
        
        render();
    }
    
    virtual void render() = 0;
    
protected:
    
    ofVec2f value;
    bool bActive;
    ofImage overlay;
    const std::map<string, PointMessage> * messages;
};
