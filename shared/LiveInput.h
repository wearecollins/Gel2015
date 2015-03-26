//
//  LiveInput.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once

#include "ofMain.h"

class LiveInput : public ofVec2f {
public:
    
    virtual void setup() = 0;
    
    virtual void update( ofVec2f val ){
        value = value * ofVec2f(.7,.7) + val * ofVec2f(.3,.3);
    }
    
    virtual void setActive( bool active = true ){
        bActive = active;
    }
    
    virtual void draw() = 0;
    
protected:
    
    ofVec2f value;
    bool bActive;
};
