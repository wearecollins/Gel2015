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
    messageTimeoutSeconds = .5;
    messageDeleteSeconds = .1;
    
    lastAverage = Poco::Timestamp();
}

//--------------------------------------------------------------
InputProcessor::~InputProcessor(){
    if ( spacebrew != NULL ){
        ofRemoveListener(spacebrew->onMessageEvent, this, &InputProcessor::onMessage);
    }
    ofRemoveListener(ofEvents().update, this, &InputProcessor::update);
}

//--------------------------------------------------------------
void InputProcessor::setup( Spacebrew::Connection & sb ){
    // attach to local spacebrew connection + subscriber(s)
    this->spacebrew = &sb;
    ofAddListener(spacebrew->onMessageEvent, this, &InputProcessor::onMessage);
    
    spacebrew->addSubscribe("average", "pad");
    spacebrew->addSubscribe("touch", "pad");
    
    // setup OF events
    ofAddListener(ofEvents().update, this, &InputProcessor::update);
}


//--------------------------------------------------------------
void InputProcessor::update( ofEventArgs & e ){
    // cleanup messages
    Poco::LocalDateTime now;
    Poco::Timespan timediff;
    
    mux.lock();
    messages.insert(messages.end(), queue.begin(), queue.end());
    queue.clear();
    mux.unlock();
    
    for ( auto it = messages.begin(); it != messages.end(); ){
//        timediff = now - m.second.time;
        timediff = now - it->time;
        if ( (float) timediff.milliseconds() / 1000.0f > messageDeleteSeconds ){
//            messages.erase(m.first);
            it = messages.erase(it);
        } else {
            it++;
        }
    }
    
    // process current value
    if ( (lastAverage.elapsed())/1000000.f < messageTimeoutSeconds ){
        bShouldSend = true;
    } else {
        bShouldSend = false;
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
    
    Poco::DateTime now;
    
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
            
            queue.push_back(p);
        }
    }
}


//--------------------------------------------------------------
bool InputProcessor::shouldSend(){
    return bShouldSend;
}
