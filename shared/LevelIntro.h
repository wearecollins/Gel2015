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
                string path = "graphics/" + name + "/" + XML.getValue();
                
                if ( path.find(".mov") != string::npos || path.find(".mp4") != string::npos ){
                    overlays.push_back( new VideoOverlay());
                    VideoOverlay* vo = dynamic_cast<VideoOverlay*>(overlays.back());
                    vo->load( path );
                } else if ( path.find(".png") == string::npos ){
                    overlays.push_back( new ImageSeqOverlay());
                    ImageSeqOverlay* vo = dynamic_cast<ImageSeqOverlay*>(overlays.back());
                    vo->load( path );
                } else {
                    overlays.push_back( new ImageOverlay());
                    ImageOverlay* lo = dynamic_cast<ImageOverlay*>(overlays.back());
                    lo->load( path );
                }
            }
            while( XML.setToSibling() && XML.getName() == "overlay" ); // go to the next overlay
        }
        
        XML.setTo("//");
        
        if(XML.exists("outro"))
        {
            string path = "graphics/" + name + "/" + XML.getValue("outro");
            outro.load( path );
            
            cout << "OUTRO LOADED "<<path<<endl;
        }
        
        // set to frame 0, but don't fade in
        setFrame(0, false);
    }
    
    void draw(){
        if ( currentFrame != NULL ){
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
            currentFrame = NULL;
        }
        setFrame(0, false);
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
    
    bool shouldDraw(){
        return bActive || ( currentFrame != NULL && currentFrame->color.a > 0);
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
    vector<Overlay *> overlays;
    
    Overlay * previousFrame;
    Overlay * currentFrame;
    
    ImageSeqOverlay outro;
    
    bool bActive;
    
    // sets current frame, fades out old (if exists), modifies index variable!
    
    void setFrame( int frameNum, bool activate = true ){
        // fade out old
        if ( currentFrame != NULL ) currentFrame->deactivate();
        previousFrame = currentFrame;
        
        index = frameNum;
        
        if ( overlays.size() != 0 ){
            currentFrame = overlays[index];
        } else {
            currentFrame = NULL;
        }
        
        if ( activate && currentFrame != NULL ) currentFrame->activate();
    }
};
