//
//  Meter.cpp
//  Bar
//
//  Created by Brett Renfer on 3/25/15.
//
//

#include "Meter.h"

//--------------------------------------------------------------
void Meter::setup(){
    // load shader + image
    meter.loadImage("graphics/level1_meter.png");
    fuzzy.loadImage("graphics/fuzzy.png");
    renderShader.load("","shaders/meterRender.frag");
    
    renderTexture.allocate(meter.width, meter.height);
    
    fillColor.set(ofRandom(255), ofRandom(255), ofRandom(255));
    fuzzy.setAnchorPercent(.5,.5);
}

//--------------------------------------------------------------
void Meter::render(){
    // draw output to FBO
    fillColor.setHue(ofWrap(ofSignedNoise(ofGetElapsedTimeMillis() * .001) * 255., 0, 255));
    fillColor.setBrightness(255);
    
    renderTexture.begin();
    ofPushMatrix();
    ofSetColor(255,10);
    ofRect(0, 0, renderTexture.getWidth(), renderTexture.getHeight());
    
    if ( bActive ){
        ofTranslate(renderTexture.getWidth()/2.0, renderTexture.getHeight());
        ofSetColor(fillColor);
        float rot = ofMap(value.x, -1., 1.0, 0, 180);
    //    float trans = ofMap(value.x, -1., 1.0, 0, 180);
        ofRotateZ(rot);
        ofTranslate(-800., 0);
        ofRotateZ(270);
        fuzzy.draw(0,0);
    }
    
    ofPopMatrix();
    renderTexture.end();
    ofSetColor(255);
    
    ofSetColor(255);
    
    renderShader.begin();
    renderShader.setUniformTexture("alpha", meter.getTextureReference(), 2);
    renderShader.setUniformTexture("render", renderTexture.getTextureReference(), 3);
    renderTexture.draw(0,0 );
    renderShader.end();
}
