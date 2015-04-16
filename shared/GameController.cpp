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
    currentLevel = LEVEL_ZERO; // debug!
    currentState = STATE_INTRO;
    bPartyMode = false;
}

//--------------------------------------------------------------
void GameController::setup( InputProcessor & input, Spacebrew::Connection & sb ){
    inputProcessor = &input;
    
    // setup each overlay
    levelInputs[LEVEL_ZERO]     = new GridMeter();
    levelInputs[LEVEL_ONE]      = new Meter();
    levelInputs[LEVEL_TWO]      = new GridMeter();
    levelInputs[LEVEL_THREE]    = new DropMeter();
    
    levelIntros[LEVEL_ZERO]     = new LevelIntro();
    levelIntros[LEVEL_ONE]      = new LevelIntro();
    levelIntros[LEVEL_TWO]      = new LevelIntro();
    levelIntros[LEVEL_THREE]    = new LevelIntro();
    
    int index = 0;
    
    for ( auto & it : levelIntros ){
        it.second->setup("level"+ ofToString(index) );
        index++;
    }
    
    // background
    colorBackground.setColor(0);
    
    // setup connection to control app
    this->spacebrew = &sb;
    spacebrew->addSubscribe("infoevent", "event" );
    spacebrew->addPublish("gameevent", "event" );
    spacebrew->addPublish("statusupdate", "event" );
    
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
    colorBackground.draw();
    
    if ( currentState != STATE_INTRO ){
        currentLive->draw();
    }
    currentIntro->draw();
}


//--------------------------------------------------------------
void GameController::triggerCelebration(){
    if ( currentState == STATE_PARTY || currentLive->bPartyMode ) return;
    currentIntro->setOutro();
    currentIntro->activate();
    currentLive->partyMode();
    
    spacebrew->send("gameevent", "event", "{\"name\":\"trigger\",\"value\":\"" + levelToString(currentLevel)+ " complete!\"}");
    currentState = STATE_PARTY;
}

//--------------------------------------------------------------
void GameController::triggerPrevLevel(){
    currentIntro->deactivate();
    currentLive->deactivate();
    
    Level next = currentLevel;
    next = (Level) ((int) currentLevel - 1);
    
    if ( next < LEVEL_ZERO ){
        next = LEVEL_ZERO;
    }
    setLevel(next);
}

//--------------------------------------------------------------
void GameController::triggerNextLevel(){
    currentIntro->deactivate();
    currentLive->deactivate();
    
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
    currentState = STATE_LIVE;

    currentIntro->deactivate();
    currentLive->activate();
    colorBackground.deactivate();
    
    spacebrew->send("gameevent", "event", "{\"name\":\"trigger\",\"value\":\"let's go\"}");
}

//--------------------------------------------------------------
void GameController::setLevel ( Level level ){
    currentState = STATE_INTRO;
    
    currentLevel = level;
    currentLive = levelInputs[ currentLevel ];
    currentIntro = levelIntros[ currentLevel ];
    currentIntro->activate();
    
    colorBackground.activate();
    
    spacebrew->send("gameevent", "event", "{\"name\":\"level\",\"value\":\"" +  levelToString( level ) + "\"}");
}


//--------------------------------------------------------------
string GameController::levelToString( Level level ){
    switch (level){
        case LEVEL_ZERO:
            return "";
            break;
            case LEVEL_ONE:
            return "level one";
            break;
            
            case LEVEL_TWO:
            return "level two";
            break;
            
            case LEVEL_THREE:
            return "level three";
            break;
    }
}

//--------------------------------------------------------------
string GameController::stateToString( State level ){
    switch (level){
        case STATE_INTRO:
            return "state intro";
            break;
        case STATE_LIVE:
            return "state live";
            break;
            
        case STATE_PARTY:
            return "state party";
            break;
    }
}

//--------------------------------------------------------------
void GameController::onMessage( Spacebrew::Message & m ){
    if ( m.name == "infoevent" ){
        // for now, just broadcast current level and if we're in party mode
        
        if ( currentState == STATE_PARTY ){
            spacebrew->send("statusupdate", "event", "{\"name\":\"trigger\",\"value\":\"" + levelToString(currentLevel)+ " complete!\"}");
        } else {
            spacebrew->send("statusupdate", "event", "{\"name\":\"level\",\"value\":\"" +  levelToString( currentLevel ) + "\"}");
        }
    }
//    Json::Reader    jsonReader;
//    Json::Value     json;
    
//    if ( m.name == "gameevent"){
//        ofStringReplace(m.value, "\\", ""); // is this still necessary?
//        bool b = jsonReader.parse( m.value, json);
//
////        {
////            name:"eventtype",
////            value:"stuff"
////        }
//        
//        if ( b ){
//            string name = json["name"].asString();
//            string value = json["value"].asString();
//            
//            if (name == "level") {
//                
//            } else if(name == "trigger"){
//                
//            }
//        }
//    }
}
