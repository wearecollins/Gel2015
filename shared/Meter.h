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
    
    Meter() : LiveInput(){
        setup();
    }
    
    void setup();
    void render();
    
protected:
    
    ofImage meter, fuzzy;
    ofShader renderShader;
    ofFbo   renderTexture;
    ofColor fillColor;
};
