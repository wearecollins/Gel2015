//
//  InputProcessor.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once

#include "ofxSpacebrew.h"
#include "Poco/LocalDateTime.h"

struct PointMessage {
    Poco::LocalDateTime time;
    ofVec2f point;
    string uniqueId;
    ofColor color;
};

class InputProcessor {
    friend class Spacebrew::Connection;
public:
    
    enum Mode {
        MODE_AVERAGE = 0,
        MODE_RANDOM_LEAD,
        MODE_LIVE
    };
    
    InputProcessor();
    ~InputProcessor();
    
    void setup( Spacebrew::Connection & spacebrew );
    void update( ofEventArgs & e );
    
    ofVec2f getCurrentValue();
    string getCurrentJSON();
    
    void setMode( Mode mode );
    Mode getMode();
    
    void setupSpacebrewMessage( string name, string type );
    
    const map<string, PointMessage> & getMessages(){
        return messages;
    }
    
    bool shouldSend();
    
    // externally modifiable settings
    float messageTimeoutSeconds;
    
protected:
    
    Mode mode;
    void onMessage( Spacebrew::Message & m );
    Spacebrew::Connection * spacebrew;
    string messageName, messageType;
    
    map<string, PointMessage> messages;
    ofVec2f currentValue;
    
    bool bShouldSend;
};