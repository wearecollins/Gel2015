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
    currentLevel = LEVEL_ONE; // debug!
}

//--------------------------------------------------------------
void GameController::setup( InputProcessor & input, Spacebrew::Connection & sb ){
    inputProcessor = &input;
    
    // setup each overlay
    levelInputs[LEVEL_ONE] = new Meter();
    levelInputs[LEVEL_TWO] = new GridMeter();
    levelInputs[LEVEL_THREE] = new DropMeter();
    
    levelIntros[LEVEL_ONE] = new LevelIntro();
    levelIntros[LEVEL_TWO] = new LevelIntro();
    levelIntros[LEVEL_THREE] = new LevelIntro();
    
    int index = 1;
    
    for ( auto & it : levelIntros ){
        it.second->setup("level"+ ofToString(index) );
        index++;
    }
    
//    levelOutros[LEVEL_ONE] = new LevelIntro();
//    levelIntros[LEVEL_TWO] = new LevelIntro();
//    levelIntros[LEVEL_THREE] = new LevelIntro();
    
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
    // maybe don't always draw these at the same time.
    currentLive->draw();
    currentIntro->draw();
}


//--------------------------------------------------------------
void GameController::triggerCelebration(){
    currentIntro->setOutro();
    currentIntro->activate();
    currentLive->partyMode();
}

//--------------------------------------------------------------
void GameController::triggerNextLevel(){
    currentIntro->deactivate();
    currentLive->deactivate();
//    currentOutro->deactivate();
    
    Level next = currentLevel;
    next = (Level) ((int) currentLevel + 1);
    
    if ( next > LEVEL_THREE ){
        next = LEVEL_THREE;
    }
    setLevel(next);
}


//--------------------------------------------------------------
void GameController::triggerNextFrame(){
    if ( currentIntro != NULL){
        currentIntro->nextFrame();
    }
}

//--------------------------------------------------------------
void GameController::triggerPrevFrame(){
    if ( currentIntro != NULL){
        currentIntro->prevFrame();
    }
}

//--------------------------------------------------------------
void GameController::triggerLive(){
    currentIntro->deactivate();
    currentLive->activate();
}

//--------------------------------------------------------------
void GameController::setLevel ( Level level ){
    currentLevel = level;
    currentLive = levelInputs[ currentLevel ];
    currentIntro = levelIntros[ currentLevel ];
    currentIntro->activate();
    
//    currentOutro = levelOutros[ currentLevel ];
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
