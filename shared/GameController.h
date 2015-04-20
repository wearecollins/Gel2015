//
//  GameController.h
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#pragma once

#include "ofMain.h"

// utils
#include "ofxUI.h"
#include "ofxSpacebrew.h"
#include "Constants.h"
#include "InputProcessor.h"
#include "Params.h"

// inputs
#include "Meter.h"
#include "GridMeter.h"
#include "DropMeter.h"

// overlays
#include "LevelIntro.h"

class GameController {
    friend class Spacebrew::Connection;
public:
    
    enum Level {
        LEVEL_ZERO = 0,
        LEVEL_ONE,
        LEVEL_TWO,
        LEVEL_THREE,
        LEVEL_END
    };
    
    enum State {
        STATE_INTRO = 0,
        STATE_LIVE,
        STATE_PARTY
    };
    
    GameController();
    
    void setup( InputProcessor & input, Spacebrew::Connection & spacebrew );
    void update( ofEventArgs & e );
    void draw();

    void guiSetup();
    void guiLayout();
    void guiToggleVisible();
    void guiToggleMinified();

    // events
    void guiEvent(ofxUIEventArgs &);
    void triggerPrevLevel();
    void triggerNextLevel();
    void triggerLive();
    void triggerCelebration();
    
    // intro-specific events
    void triggerNextFrame();
    void triggerPrevFrame();
    
protected:
    
    Level   currentLevel;
    State   currentState;
    bool    bPartyMode;
    
    Overlay                 colorBackground;
    LevelIntro *            currentIntro;
    LiveInput *             currentLive;
//    CelebrationOverlay *    currentOutro;
    
    map<Level, LevelIntro *>            levelIntros;
    map<Level, LiveInput *>             levelInputs;

    vector<ofxUISuperCanvas*> guiPanels;
    
    // internal mechanics
    InputProcessor * inputProcessor;
    void setLevel ( Level level );
    void saveState();
    void setBackgroundTransparent(bool transparent = true);
    
    string levelToString( Level level );
    string stateToString( State level );
    
    // connection to controller app
    Spacebrew::Connection * spacebrew;
    void onMessage( Spacebrew::Message & message );
    
    // send out pings to phones regularly
    int statusSendMillis;
    Poco::Timestamp lastStatusSent;
};
