//
//  Constants.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once

#include "ofMain.h"
#include "Poco/LocalDateTime.h"

static float getProjectorWidth(){
    return 1920.;
}

static float getProjectorHeight(){
    return 1080.;
}

struct PointMessage {
    Poco::LocalDateTime time;
    ofVec2f point;
    string uniqueId;
    ofColor color;
};