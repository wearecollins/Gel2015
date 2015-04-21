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

class InputProcessor : protected ofThread {
    friend class Spacebrew::Connection;
public:
    
    InputProcessor();
    ~InputProcessor();
    
    void setup( Spacebrew::Connection & spacebrew );
    
    int getCurrentValue();
    int getCurrentPower();
    
//    const map<string, PointMessage> & getMessages(){
//        return messages;
//    }

    vector<PointMessage> & getMessages(){
        safeMessages.clear();
        lock();
        safeMessages.insert(safeMessages.begin(), messages.begin(), messages.end());
        unlock();
        
        return safeMessages;
    }
    
    void doEasing( bool bDo = true );
    
    bool shouldSend();
    
    Poco::Timestamp lastAverageReceived();
    
    // externally modifiable settings
    float messageTimeoutSeconds, messageDeleteSeconds;
    
protected:
    
    void threadedFunction();
    
    void onMessage( Spacebrew::Message & m );
    Spacebrew::Connection * spacebrew;
    string messageName, messageType;
    
    // dump of all current messages
    vector<PointMessage> safeMessages;
    vector<PointMessage> messages;
    vector<PointMessage> queue;
    
    // updated live from average
    int currentValue;
    int currentPower;
    Poco::Timestamp lastAverage; // last time we got an "average" value
    
    bool bShouldSend, bEaseValue;
    
    ofMutex mux;
};