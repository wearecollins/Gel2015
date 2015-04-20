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

//#define DEBUG_ZONE

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
        pnt.set(ofMap(value, 1,2, getProjectorWidth()/2., getProjectorWidth()),
                ofMap(value, 1,2, 0, getProjectorHeight()/2.0));
        
    } else if ( value >= 2 && value < 3 ){
        
        pnt.set(ofMap(value, 2,3, getProjectorWidth(), getProjectorWidth()/2.),
                ofMap(value, 2,3, getProjectorHeight()/2.0, getProjectorHeight()));
        
    } else if ( value >= 3 ){
        pnt.set(ofMap(value, 3,4, getProjectorWidth(), getProjectorWidth()/2.),
                ofMap(value, 3,4, getProjectorHeight()/2.0, getProjectorHeight()));
        
    }
    return pnt;
}

class PointMessage {
public:
    ~PointMessage(){};
    Poco::Timestamp time;
    ofVec2f point; // deprecated
    string uniqueId;
    ofColor color;
    int     direction;  //0-3 = L U R D
    int     power;      //0-2, only used in voice app
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