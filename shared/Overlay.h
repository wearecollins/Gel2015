//
//  Overlay.h
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class Overlay : public ofVec2f {
    friend class LevelIntro;
public:
    
    Overlay(){
        color.a = 0;
        bActive = true;
    };
    
    virtual void activate( float level = 255 ){
        bActive = true;
        targetAlpha = level;
    }
    
    virtual void deactivate(){
        bActive = false;
    }
    
    virtual void draw(){
        if ( !bActive ){
            color.a *= .9;
        } else {
            //color.a = color.a * .9 + targetAlpha * .1;
        }
        ofPushStyle();
        ofSetColor(color);
        ofRect(0,0,getProjectorWidth(), getProjectorHeight());
        ofPopStyle();
    }
    
    virtual void setColor( float colorR, float colorG = -1, float colorB=-1, float colorA = 255){
        if ( colorG == -1 ){
            color.set(colorR);
        } else if ( colorB == -1 ){
            color.set(colorR, colorG);
        } else {
            color.set(colorR, colorG, colorB, colorA );
        }
    }
    
protected:
    ofColor color;
    float targetAlpha;
    bool bActive;
};

/**
 @class ImageOverlay
*/
class ImageOverlay : public Overlay {
public:
    void load( string image ){
        color.set(255,0);
        overlay.loadImage(image);
    }
    
    
    void draw(){
        if ( !bActive ){
            color.a *= .9;
        } else {
            color.a = color.a * .9 + targetAlpha * .1;
        }
        
        ofPushStyle();
        ofSetColor(color);
        if ( overlay.isAllocated() ) overlay.draw(0,0);
        ofPopStyle();
    }
    
    
protected:
    ofImage overlay;
};

/**
 @class VideoOverlay
 */
class VideoOverlay : public Overlay {
public:
    void load( string video ){
        color.set(255,0);
        overlay.loadMovie(video, OF_QTKIT_DECODE_TEXTURE_ONLY);
        overlay.setLoopState(OF_LOOP_NORMAL);
        overlay.play();
        overlay.stop();
    }
    
    void activate( float level = 255 ){
        bActive = true;
        targetAlpha = level;
        overlay.play();
    }
    
    void deactivate(){
        bActive = false;
        overlay.stop();
    }
    
    void draw(){
        if ( !bActive ){
            color.a *= .9;
        } else {
            color.a = color.a * .9 + targetAlpha * .1;
        }
        
        ofPushStyle();
        ofSetColor(color);
        overlay.draw(0,0);
        ofPopStyle();
    }
    
    
protected:
    ofQTKitPlayer overlay;
};
