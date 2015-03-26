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

#include "InputProcessor.h"

// inputs
#include "LiveInput.h"
#include "Meter.h"

class GameController {
public:
    
    enum Level {
        LEVEL_ONE = 0,
        LEVEL_TWO,
        LEVEL_THREE
    };
    
    GameController();
    
    void setup( InputProcessor & input );
    void update( ofEventArgs & e );
    void draw();
    
    // events
    void triggerCelebration();
    void triggernextLevel();
    
protected:
    
    Level currentLevel;
    
    // LevelOverlay *       currentIntro;
    LiveInput *             currentLive;
    // CelebrationOverlay * currentOutro;
    
    //map<Level, LevelOverlay *>        levelOverlays;
    map<Level, LiveInput *>             levelInputs;
    //map<Level, CelebrationOverlay *>  levelOutros;
    
    // internal mechanics
    InputProcessor * inputProcessor;
    void setLevel ( Level level );
};
