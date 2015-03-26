//
//  Meter.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once
#include "LiveInput.h"

class Meter : public LiveInput {
public:
    
    Meter(){
        setup();
    }
    
    void setup();
    void draw();
    
protected:
    
    ofImage meter, fuzzy, overlay;
    ofShader render;
    ofFbo   renderTexture;
    ofColor fillColor;
};
