//
//  InputProcessor.cpp
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#include "InputProcessor.h"

//--------------------------------------------------------------
InputProcessor::InputProcessor(){
    spacebrew = NULL;
    bShouldSend = false;
    bEaseValue = true;
    currentValue = 0;
    currentPower = 0;
    messageTimeoutSeconds = 1.;
    messageDeleteSeconds = .1;
    
    lastAverage = Poco::Timestamp();
}

//--------------------------------------------------------------
InputProcessor::~InputProcessor(){
    if ( spacebrew != NULL ){
        ofRemoveListener(spacebrew->onMessageEvent, this, &InputProcessor::onMessage);
    }
    if (isThreadRunning()){
        waitForThread();
    }
}

//--------------------------------------------------------------
void InputProcessor::setup( Spacebrew::Connection & sb ){
    // attach to local spacebrew connection + subscriber(s)
    this->spacebrew = &sb;
    ofAddListener(spacebrew->onMessageEvent, this, &InputProcessor::onMessage);
    
    spacebrew->addSubscribe("average", "pad");
    spacebrew->addSubscribe("touch", "pad");
    
    // let's go
    startThread();
}


//--------------------------------------------------------------
void InputProcessor::threadedFunction(){
    while (isThreadRunning()){
    // cleanup messages
    
        lock();
        messages.insert(messages.end(), queue.begin(), queue.end());
        queue.clear();
        unlock();
        
        std::list<vector<PointMessage>::iterator > itList;
        
        for ( auto it = messages.begin(); it != messages.end(); ++it){
            if ( it->time.elapsed()/1000000.f > messageDeleteSeconds ){
    //            messages.erase(m.first);
                itList.push_back(it);
            }
        }
        for ( auto it : itList ){
            messages.erase(it);
        }
        
        // process current value
        if ( (lastAverage.elapsed())/1000000.f < messageTimeoutSeconds ){
            bShouldSend = true;
        } else {
            bShouldSend = false;
        }
        sleep(16);
        yield();
    }
}

//--------------------------------------------------------------
int InputProcessor::getCurrentValue(){
    return currentValue;
}

//--------------------------------------------------------------
int InputProcessor::getCurrentPower(){
    return currentPower;
}

//--------------------------------------------------------------
void InputProcessor::doEasing( bool bDo ){
    bEaseValue = bDo;
}

//--------------------------------------------------------------
void InputProcessor::onMessage( Spacebrew::Message & m ){
    static Json::Reader reader;
    Json::Value root;
    
    Poco::Timestamp now;
    
    if ( m.name == "average" ){
        vector<string> exp = ofSplitString(m.value, ":");
        
        if ( exp.size() >= 2 ){
            int dir = ofToInt(exp[1]);
            string ID = exp[0];
            currentValue = dir;
            lastAverage = Poco::Timestamp();
            if ( exp.size() > 2 ){
                currentPower = ofToInt(exp[2]);
            }
        }
        
    } else if ( m.name == "touch" ){
        
        vector<string> exp = ofSplitString(m.value, ":");
        
        if ( exp.size() >= 2 ){
            int dir = ofToInt(exp[0]);
            string ID = exp[1];
//            if ( messages.count(ID) == 0 ){
//                messages[ID] = PointMessage();
//            }
            
            PointMessage p;
//            messages[ID].time       = now;
//            messages[ID].uniqueId   = ID;
//            messages[ID].direction  = dir;
            p.time       = now;
            p.uniqueId   = ID;
            p.direction  = dir;
            
            lock();
            queue.push_back(p);
            unlock();
        }
    }
}


//--------------------------------------------------------------
bool InputProcessor::shouldSend(){
    return bShouldSend;
}

//--------------------------------------------------------------
Poco::Timestamp InputProcessor::lastAverageReceived(){
    return lastAverage;
}
