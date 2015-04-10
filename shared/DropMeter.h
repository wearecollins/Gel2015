//
//  DropMeter.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once 

#include "LiveInput.h"
#include "DropPoint.h"

class DropMeter : public LiveInput {
public:
    
    DropMeter();
    
    void setup();
    void render();
    void partyMode();
    
protected:
    vector<DropPoint*> grid;
};