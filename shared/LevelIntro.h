//
//  LevelIntro.h
//  GelOutput
//
//  Created by Brett Renfer on 4/2/15.
//
//

#pragma once

#include "ofMain.h"
#include "Overlay.h"

class LevelIntro {
public:
    
    LevelIntro(){
        index = 0;
        previousFrame = NULL;
        currentFrame = NULL;
        bActive = false;
    }
    
    /**
     * @param {string} name Name of directory we're loading from
     */
    void setup( string name ){
        ofXml XML;
        XML.load( "graphics/" + name + "/settings.xml" );
        
        if(XML.exists("overlay"))
        {
            
            // This gets the first stroke (notice the [0], it's just like an array)
            XML.setTo("overlay[0]");
            
            do {
                overlays.push_back( Overlay());
                string path = "graphics/" + name + "/" + XML.getValue();
                overlays.back().load( path );
            }
            while( XML.setToSibling() && XML.getName() == "overlay" ); // go to the next overlay
        }
        
        XML.setTo("//");
        
        if(XML.exists("outro"))
        {
            string path = "graphics/" + name + "/" + XML.getValue("outro");
            outro.load( path );
        }
        
        // set to frame 0, but don't fade in
        setFrame(0, false);
    }
    
    void draw(){
        
        if ( currentFrame != NULL ){
            
//            ofSetColor(0,255);
//            ofRect(0,0, getProjectorWidth(), getProjectorHeight());
            ofSetColor(255);
            
            currentFrame->draw();
        }
        
        if ( previousFrame != NULL ){
            previousFrame->draw();
        }
    }
    
    void activate(){
        bActive = true;
        if ( currentFrame != NULL ){
            currentFrame->activate();
        }
    }
    
    void deactivate(){
        bActive = false;
        if ( currentFrame != NULL ){
            currentFrame->deactivate();
        }
    }
    
    void nextFrame(){
        if (index + 1 >= overlays.size()) return;
        setFrame( index + 1, bActive );
    }
    
    void prevFrame(){
        if (index - 1 < 0) return;
        setFrame( index -1, bActive );
    }
    
    void setOutro( bool activate = true){
        // fade out old
        if ( currentFrame != NULL ) currentFrame->deactivate();
        previousFrame = currentFrame;
        
        currentFrame = &outro;
        if ( activate ) currentFrame->activate();
    }
    
    bool isLastFrame(){
        return index == (overlays.size() - 1);
    }
    
    bool isActive(){
        return bActive;
    }
    
    Overlay & getOutro(){
        return outro;
    }
    
    Overlay * getOutroPtr(){
        return &outro;
    }
    
protected:
    int index;
    
    // this should be a vector of pointers
    // to enable MovOverlay, etc
    vector<Overlay> overlays;
    
    Overlay * previousFrame;
    Overlay * currentFrame;
    
    Overlay outro;
    
    bool bActive;
    
    // sets current frame, fades out old (if exists), modifies index variable!
    
    void setFrame( int frameNum, bool activate = true ){
        // fade out old
        if ( currentFrame != NULL ) currentFrame->deactivate();
        previousFrame = currentFrame;
        
        index = frameNum;
        
        currentFrame = &overlays[index];
        if ( activate ) currentFrame->activate();
    }
};
