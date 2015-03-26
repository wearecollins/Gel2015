//
//  GameController.cpp
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#include "GameController.h"

//--------------------------------------------------------------
GameController::GameController(){
    currentLevel = LEVEL_TWO; // debug!
}

//--------------------------------------------------------------
void GameController::setup( InputProcessor & input, Spacebrew::Connection & sb ){
    inputProcessor = &input;
    
    // setup each overlay
    levelInputs[LEVEL_ONE] = new Meter();
    levelInputs[LEVEL_TWO] = new GridMeter();
    //levelInputs[LEVEL_THREE] = new Meter();
    
    // setup connection to control app
    this->spacebrew = &sb;
    spacebrew->addSubscribe("gameevent", "event" );
    
    // let's go
    setLevel( currentLevel );
    
    // setup OF events
    ofAddListener(ofEvents().update, this, &GameController::update);
    ofAddListener(this->spacebrew->onMessageEvent, this, &GameController::onMessage);
}

//--------------------------------------------------------------
void GameController::update( ofEventArgs & e ){
    // connect input to live meter
    currentLive->update( inputProcessor->getCurrentValue());
    currentLive->updateAll( inputProcessor->getMessages() );
    currentLive->setActive( inputProcessor->shouldSend() );
}

//--------------------------------------------------------------
void GameController::draw(){
    currentLive->draw();
    
//    currentIntro->draw();
//    currentOutro->draw();
}


//--------------------------------------------------------------
void GameController::triggerCelebration(){
    
}

//--------------------------------------------------------------
void GameController::triggernextLevel(){
    
}

//--------------------------------------------------------------
void GameController::setLevel ( Level level ){
    currentLevel = level;
    currentLive = levelInputs[ currentLevel ];
}

//--------------------------------------------------------------
void GameController::onMessage( Spacebrew::Message & m ){
    Json::Reader    jsonReader;
    Json::Value     json;
    
    if ( m.name == "gameevent"){
        ofStringReplace(m.value, "\\", ""); // is this still necessary?
        bool b = jsonReader.parse( m.value, json);

//        {
//            name:"eventtype",
//            value:"stuff"
//        }
        
        if ( b ){
            string name = json["name"].asString();
            string value = json["value"].asString();
            
            if (name == "level") {
                
            } else if(name == "trigger"){
                
            }
        }
    }
}
