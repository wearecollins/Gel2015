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

static ofVec2f getGridPoint( float value ){
    ofVec2f pnt;
    if ( value < 1 ){
        pnt.set(ofMap(value, 0,1, 0, getProjectorWidth()/2.),
                ofMap(value, 0,1, getProjectorHeight()/2.0, 0));
    } else if ( value >= 1 && value < 2 ){
        pnt.set(ofMap(value, 0,1, 0, getProjectorWidth()/2.),
                ofMap(value, 0,1, getProjectorHeight()/2.0, 0));
        
    } else if ( value >= 2 && value < 3 ){
        
        pnt.set(ofMap(value, 2,3, getProjectorWidth(), getProjectorWidth()/2.),
                ofMap(value, 2,3, getProjectorHeight()/2.0, getProjectorHeight()));
        
    } else if ( value >= 3 ){
        pnt.set(ofMap(value, 2,3, getProjectorWidth(), getProjectorWidth()/2.),
                ofMap(value, 2,3, getProjectorHeight()/2.0, getProjectorHeight()));
        
    }
    return pnt;
}

struct PointMessage {
    Poco::LocalDateTime time;
    ofVec2f point;
    string uniqueId;
    ofColor color;
    int     direction;
};

class Settings {
public:
    
    static Settings& get()
    {
        static Settings instance; // Guaranteed to be destroyed.
        static bool bInstance = false;
        
        if ( !bInstance ){
            ofXml settings;
            settings.load("settings.xml");
            
            bInstance = true;
        }
        
        return instance;
    }
    
private:
    
    Settings(){};
    
    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    //Settings(Settings const&)               = delete;
    //void operator=(Settings const&)  = delete;
};