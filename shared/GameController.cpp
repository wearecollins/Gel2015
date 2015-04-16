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

    guiSetup();

    // let's go
    setLevel( currentLevel );
    
    // setup OF events
    ofAddListener(ofEvents().update, this, &GameController::update);
    ofAddListener(this->spacebrew->onMessageEvent, this, &GameController::onMessage);
}

//--------------------------------------------------------------
void GameController::guiSetup(){
    ofxUISuperCanvas* gui;

    gui = new ofxUISuperCanvas("Level 1");
    gui->addSpacer();

    gui->addSlider("Party Segment Min", 0, 300, &Params::level1partyModeSegmentLengthMin);
    gui->addSlider("Party Segment Max", 0, 300, &Params::level1partyModeSegmentLengthMax);
    gui->addSlider("Party Speed", 0, 10, &Params::level1partySpeed);

    guiPanels.push_back(gui);


    gui = new ofxUISuperCanvas("Level 2");
    gui->addSpacer();

    gui->addIntSlider("Grid Steps X", 10, 50, &Params::level2gridStepsX);
    gui->addIntSlider("Grid Steps Y", 10, 50, &Params::level2gridStepsY);
    gui->addSpacer();

    gui->addSlider("Arrow Thickness", 0, 300, &Params::level2arrowThickness);
    gui->addLabelButton("Edit Arrows", false);

    guiPanels.push_back(gui);


    for (int i = 0; i < guiPanels.size(); i++) {
        guiPanels[i]->autoSizeToFitWidgets();
        ofAddListener(guiPanels[i]->newGUIEvent, this, &GameController::guiEvent);
    }
    guiLayout();

}

//--------------------------------------------------------------
void GameController::guiLayout(){
    for (int i = 0; i < guiPanels.size(); i++) {
        if (i == 0)
            guiPanels[i]->setPosition(0, 30);
        else
            guiPanels[i]->setPosition(0, guiPanels[i-1]->getRect()->getMaxY()+1);
    }
}

//--------------------------------------------------------------
void GameController::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();

    // check for panel minimize/expand event
    if (name == "OFX_UI_WIDGET_CANVAS") {
        guiLayout();
    }

    if (e.getParent() == guiPanels[0]) {
        Meter* meter = (Meter*) levelInputs[LEVEL_ONE];

        if (name == "Party Segment Min" || name == "Party Segment Max")
            meter->partyMode();
    }

    if (e.getParent() == guiPanels[1]) {
        GridMeter* meter = (GridMeter*) levelInputs[LEVEL_TWO];

        if (name == "Grid Steps X" || name == "Grid Steps Y")
            meter->setupGrid();
        if (name == "Edit Arrows")
            meter->editArrows();
        if (name == "Arrow Thickness") {
            meter->calcArrowInnerEndpoints();
            meter->calcArrowThickness();
        }

    }
}

//--------------------------------------------------------------
void GameController::guiToggleVisible(){
    for (int i = 0; i < guiPanels.size(); i++) {
        guiPanels[i]->toggleVisible();
    }
}

//--------------------------------------------------------------
void GameController::guiToggleMinified(){

    bool minify = false;

    for (int i = 0; i < guiPanels.size(); i++) {
        if (!guiPanels[i]->isMinified()) {
            minify = true;
            break;
        }
    }

    for (int i = 0; i < guiPanels.size(); i++) {
        guiPanels[i]->setMinified(minify);
    }

    guiLayout();
    
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
            spacebrew->send("gameevent", "event", "{\"name\":\"trigger\",\"value\":\"" + levelToString(currentLevel)+ " complete!\"}");
        } else {
            spacebrew->send("gameevent", "event", "{\"name\":\"level\",\"value\":\"" +  levelToString( currentLevel ) + "\"}");
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
