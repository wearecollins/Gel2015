//
//  InputProcessor.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once

#include "Constants.h"
#include "ofxSpacebrew.h"

class InputProcessor {
    friend class Spacebrew::Connection;
public:
    
    InputProcessor();
    ~InputProcessor();
    
    void setup( vector<Spacebrew::Connection *> & sb );
    void update( ofEventArgs & e );
    
    int getCurrentValue();
    int getCurrentPower();
    
//    const map<string, PointMessage> & getMessages(){
//        return messages;
//    }

    const vector<PointMessage> & getMessages(){
        return messages;
    }
    
    void doEasing( bool bDo = true );
    
    bool shouldSend();
    
    // externally modifiable settings
    float messageTimeoutSeconds, messageDeleteSeconds;
    
protected:
    void onMessage( Spacebrew::Message & m );
    vector<Spacebrew::Connection *> * spacebrew;
    string messageName, messageType;
    
    // dump of all current messages
    vector<PointMessage> messages;
    
    // updated live from average
    int currentValue;
    int currentPower;
    Poco::Timestamp lastAverage; // last time we got an "average" value
    
    bool bShouldSend, bEaseValue;
    
    ofMutex mux;
};