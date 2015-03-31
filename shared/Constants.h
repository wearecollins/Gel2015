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