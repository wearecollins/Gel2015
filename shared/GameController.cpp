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
    currentLevel = LEVEL_ONE;
}

//--------------------------------------------------------------
void GameController::setup( InputProcessor & input ){
    inputProcessor = &input;
    
    // setup each overlay
    levelInputs[LEVEL_ONE] = new Meter();
    //levelInputs[LEVEL_TWO] = new Meter();
    //levelInputs[LEVEL_THREE] = new Meter();
    
    // let's go
    setLevel( currentLevel );
    
    // setup OF events
    ofAddListener(ofEvents().update, this, &GameController::update);
}

//--------------------------------------------------------------
void GameController::update( ofEventArgs & e ){
    // connect input to live meter
    currentLive->update( inputProcessor->getCurrentValue());
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