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
#include "ofxSpacebrew.h"
#include "Constants.h"
#include "InputProcessor.h"

// inputs
#include "Meter.h"
#include "GridMeter.h"
#include "DropMeter.h"

// overlays
#include "LevelIntro.h"
//#include "CelebrationOverlay.h"

class GameController {
    friend class Spacebrew::Connection;
public:
    
    enum Level {
        LEVEL_ONE = 0,
        LEVEL_TWO,
        LEVEL_THREE
    };
    
    GameController();
    
    void setup( InputProcessor & input, Spacebrew::Connection & spacebrew );
    void update( ofEventArgs & e );
    void draw();
    
    // events
    void triggerNextLevel();
    void triggerLive();
    void triggerCelebration();
    
    // intro-specific events
    void triggerNextFrame();
    void triggerPrevFrame();
    
protected:
    
    Level currentLevel;
    
    LevelIntro *            currentIntro;
    LiveInput *             currentLive;
//    CelebrationOverlay *    currentOutro;
    
    map<Level, LevelIntro *>            levelIntros;
    map<Level, LiveInput *>             levelInputs;
//    map<Level, CelebrationOverlay *>    levelOutros;
    
    // internal mechanics
    InputProcessor * inputProcessor;
    void setLevel ( Level level );
    
    string levelToString( Level level );
    
    // connection to controller app
    Spacebrew::Connection * spacebrew;
    void onMessage( Spacebrew::Message & message );
};
