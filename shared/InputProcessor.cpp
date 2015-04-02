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
    mode = MODE_AVERAGE;
    messageName = "touch";
    messageType = "gelpoint";
    messageTimeoutSeconds = .2;
    
    
    bShouldSend = false;
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
    
    spacebrew->addSubscribe(messageName, messageType);
    
    // setup OF events
    ofAddListener(ofEvents().update, this, &InputProcessor::update);
}


//--------------------------------------------------------------
void InputProcessor::update( ofEventArgs & e ){
    // cleanup messages
    Poco::LocalDateTime now;
    Poco::Timespan timediff;
    
    for ( auto & m : messages ){
        timediff = now - m.second.time;
        if ( (float) timediff.milliseconds() / 1000.0f > messageTimeoutSeconds ){
            messages.erase(m.first);
        }
    }
    
    // process current value
    if ( messages.size() > 0 ){
        switch (mode) {
            case MODE_AVERAGE:
            {
                ofVec2f total;
                for ( auto & m : messages ){
                    total += m.second.point;
                }
                total /= messages.size();
                currentValue = total;
            }
                break;
                
            case MODE_RANDOM_LEAD:
            {
                // first average
                ofVec2f total;
                for ( auto & m : messages ){
                    total += m.second.point;
                }
                total /= messages.size();
                
                int index = floor(ofRandom(messages.size()));
                
                auto it = messages.begin();
                std::advance(it, index);
                
                total.x = total.x * .5 + it->second.point.x * .5;
                total.x = total.x * .5 + it->second.point.x * .5;
                currentValue = total;
            }
                break;
                
            case MODE_LIVE:
            {
                // found the most recent point
                int least = FLT_MAX;
                ofVec2f curr;
                for ( auto & m : messages ){
                    timediff = now - m.second.time;
                    if ( timediff.milliseconds() < least ){
                        curr = m.second.point;
                        least = timediff.milliseconds();
                    }
                }
                currentValue = curr;
            }
                break;
                
            default:
                break;
        }
        bShouldSend = true;
    } else {
        bShouldSend = false;
        // drift back to 0
        currentValue.x = currentValue.x * .9;
        currentValue.y = currentValue.y * .9;// + -1 * .1;
    }
}

//--------------------------------------------------------------
void InputProcessor::setupSpacebrewMessage( string name, string type ){
    string last = messageName;
    messageName = name;
    
    if ( last != messageName ){
        spacebrew->addSubscribe(messageName, messageType);
    }
}

//--------------------------------------------------------------
ofVec2f InputProcessor::getCurrentValue(){
    return currentValue;
}


//--------------------------------------------------------------
string InputProcessor::getCurrentJSON(){
    return "{\"x\":" + ofToString(currentValue.x, 5) + ",\"y\":" + ofToString(currentValue.y, 5) + "}";
}

//--------------------------------------------------------------
void InputProcessor::setMode( Mode mode ){
    this->mode = mode;
}

//--------------------------------------------------------------
InputProcessor::Mode InputProcessor::getMode(){
    return this->mode;
}

//--------------------------------------------------------------
void InputProcessor::onMessage( Spacebrew::Message & m ){
    static Json::Reader reader;
    Json::Value root;
    
    if ( m.name == messageName && m.type == messageType ){
        Poco::DateTime now;
        
        ofStringReplace(m.value, "\\", "");
        bool b = reader.parse( m.value, root);
        if ( b ){
            if ( root.isObject() && root.isMember("id") ){
                string ID = root["id"].asString();
                if ( messages.count(ID) == 0 ){
                    messages[ID] = PointMessage();
                }
                
                messages[ID].time       = now;
                messages[ID].uniqueId   = ID;
                
                if ( !root["x"].isNull() ){
                    messages[ID].point.x = ofToFloat(root["x"].asString());
                    messages[ID].point.y = ofToFloat(root["y"].asString());
                }
                
                if ( root["color"].isObject()){
                    messages[ID].color.set(root["color"]["r"].asInt(), root["color"]["g"].asInt(), root["color"]["b"].asInt());
                }
            } else {
            }
        } else {
        }
    }
}


//--------------------------------------------------------------
bool InputProcessor::shouldSend(){
    return bShouldSend;
}
