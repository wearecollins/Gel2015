//
//  GridMeter.cpp
//  GelOutput
//
//  Created by Brett Renfer on 3/26/15.
//
//

#include "GridMeter.h"

#pragma mark GridPoint

//--------------------------------------------------------------
GridPoint::GridPoint(){
    radius = 50;
    color.set(255, 0);
}

//--------------------------------------------------------------
void GridPoint::draw(){
    color.a *= .9;

    ofPushMatrix();
    ofPushStyle();
    ofFill();
    ofSetColor(color);
    ofCircle(*this, radius);
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void GridPoint::activate( float level, bool force ){
    if (color.a < level || force) {
        color.a = level;
    }
}


#pragma mark GridMeter

//--------------------------------------------------------------
GridMeter::GridMeter()  : LiveInput(){
    setup();
}

//--------------------------------------------------------------
void GridMeter::setup(){
    ofRegisterMouseEvents(this);
    setupGrid();
    
    pulseRateSeconds = .5;
    
    arrows.resize(3);

    // left arrow
    arrows[0].pointA.set(10, 540);
    arrows[0].pointB.set(290, 262);
    arrows[0].pointF.set(290, 818);

    // top arrow
    arrows[1].pointA.set(953, 8);
    arrows[1].pointB.set(1206, 242);
    arrows[1].pointF.set(702, 240);

    // right arrow
    arrows[2].pointA.set(1899, 540);
    arrows[2].pointB.set(1633, 281);
    arrows[2].pointF.set(1633, 799);

    calcArrowInnerEndpoints();
    calcArrowThickness();

//    pulses.resize(1);
//    pulses[0].shape.setWidth(100);
//    pulses[0].shape.setHeight(600);

    bWaitingForArrowStartClick = false;
    bWaitingForArrowEndClick = false;
}

//--------------------------------------------------------------
void GridMeter::setupGrid(){

    // grid of 300?
    float space = 10;
    float w = getProjectorWidth();
    float h = getProjectorHeight();
    
    float x_spacing = (float) w / Params::level2gridStepsX;
    float y_spacing = (float) h / Params::level2gridStepsY;
    
    float rad = min( x_spacing - space, y_spacing - space ) / 2.0;

    grid.clear();
    for (int x=0; x<Params::level2gridStepsX; x++){
        for (int y=0; y<Params::level2gridStepsY; y++){
            grid.push_back(GridPoint());
            grid.back().set( rad + x * x_spacing, rad + y * y_spacing );
            grid.back().radius = rad;
            grid.back().color.setSaturation(255);
            grid.back().color.setBrightness(255);
        }
    }
}

//--------------------------------------------------------------
void GridMeter::render(){
    renderTexture.begin();
    ofClear(255);
    ofPushMatrix();
    
    Poco::LocalDateTime now;
    Poco::Timespan timediff;
    
    // update pulses
    for (auto &p :pulses ){
        p.anim->update(1./60.);
        switch (p.dir) {
            case 0:
            case 2:
                p.shape.x = p.anim->val();
                break;
                
            case 1:
                p.shape.y = p.anim->val();
                break;
        }
//        ofRect(p.shape); // debug zones
    }
    
    // radius of "hit" based on grid point
    float rad = 300;
    
    if ( messages != NULL ){
        for (auto & g : grid ){
            for ( auto & m : *messages ){
                ofVec2f pnt2 = getGridPoint(m.direction);
                if ( abs( g.distance(pnt2)) < rad/4. && bActive ){
                    g.activate(50);
                }
            }
        }
    }
    
    // Fire off pulses if necessary
    if ( bActive ){
        int absValue = round(value);
        
        if ( absValue < arrows.size()){
            
            timediff = now - arrows[absValue].lastPulse;
            // annoying math, but seconds == int so...
            if ( timediff.milliseconds() / 1000.f > pulseRateSeconds ){
                arrows[absValue].lastPulse = now;
                pulses.push_back(Pulse());
                
                pulses.back().dir = absValue;
                
                switch (absValue) {
                    case 0:
                    {
                        pulses.back().shape.setWidth(100);
                        pulses.back().shape.setHeight(600);
                        pulses.back().shape.x = arrows[absValue].pointB.x + 20;// whatever
                        pulses.back().shape.y = arrows[absValue].pointB.y;
                        
                        pulses.back().anim = new ofxAnimatableFloat();
                        pulses.back().anim->reset(arrows[absValue].pointB.x);
                        pulses.back().anim->setCurve(LINEAR);
                        pulses.back().anim->setDuration( pulseRateSeconds );
                        pulses.back().anim->animateTo(arrows[absValue].pointA.x - pulses.back().shape.width );
                    }
                        break;
                        
                    case 1:
                    {
                        pulses.back().shape.setWidth(600);
                        pulses.back().shape.setHeight(100);
                        pulses.back().shape.x = arrows[absValue].pointF.x;
                        pulses.back().shape.y = arrows[absValue].pointF.y;
                        
                        pulses.back().anim = new ofxAnimatableFloat();
                        pulses.back().anim->reset(arrows[absValue].pointF.y);
                        pulses.back().anim->setCurve(LINEAR);
                        pulses.back().anim->setDuration( pulseRateSeconds );
                        pulses.back().anim->animateTo(arrows[absValue].pointA.y - pulses.back().shape.height );
                    }
                        break;
                        
                    case 2:
                    {
                        pulses.back().shape.setWidth(100);
                        pulses.back().shape.setHeight(600);
                        pulses.back().shape.x = arrows[absValue].pointB.x - 20;// whatever
                        pulses.back().shape.y = arrows[absValue].pointB.y;
                        
                        pulses.back().anim = new ofxAnimatableFloat();
                        pulses.back().anim->reset(arrows[absValue].pointB.x);
                        pulses.back().anim->setCurve(LINEAR);
                        pulses.back().anim->setDuration( pulseRateSeconds );
                        pulses.back().anim->animateTo(arrows[absValue].pointA.x + pulses.back().shape.width );
                    }
                        break;
                }
                
                
            }
        }
    }
    
    for (auto & g : grid ){
        if (!bPartyMode) {
            // activate based on arrows/pulses
            for (auto& arrow : arrows) {
                if (arrow.shape.inside(g)) {
                    for (auto& p : pulses) {
                        if (p.shape.inside(g)) {
                            g.color.set(GEL_COLORS[p.dir]);
                            g.activate(200);
                            break;
                        }
                    }
                }
            }
        } else {
            // partaaaayyyy
            float noiseVal = ofNoise(g.x * Params::level2partyNoiseStepSize, g.y * Params::level2partyNoiseStepSize, ofGetElapsedTimef() * Params::level2partySpeed);
            g.activate(noiseVal * 255, true);

            // dim out the dot based on its distance to the center
            float dist = ofDist(g.x, g.y, 970, 550);
            if (dist < Params::level2partyVignetteRadius) {
                g.color.a *= ofMap(dist, 0, Params::level2partyVignetteRadius, 0, 1, true);
            }
        }
        g.draw();
    }
    
    // clean up pulses
    
    for (auto it = pulses.begin(); it != pulses.end(); ) {
        if ( it->anim->hasFinishedAnimating() ){
            pulses.erase(it);
        } else {
            ++it;
        }
    }

    // for "editing" the arrow endpoints
    ofPushStyle();

    ofNoFill();
    ofSetColor(ofColor::yellow);
    if ( bWaitingForArrowStartClick || bWaitingForArrowEndClick){
    }
    ofSetColor(ofColor::cyan, 150);
    if (bWaitingForArrowStartClick) {
        ofFill();
        ofCircle(arrows[0].pointA, 15);  // point of left arrow where mouse is
        ofCircle(arrows[1].pointA, 15);  // point of right arrow is mirrored mouse point
        ofCircle(arrows[2].pointA, 15);  // point of top arrow is mouse distance from left edge
    } else if (bWaitingForArrowEndClick || Params::level2drawArrows) {
        ofNoFill();
        ofSetLineWidth(10);
        for (auto& arrow : arrows) arrow.shape.draw();
    }

    ofPopStyle();

    ofPopMatrix();
    renderTexture.end();
    
    ofSetColor(255, fill.a);
    
    renderTexture.draw(0,0);
}

//--------------------------------------------------------------
void GridMeter::partyMode(){
    LiveInput::partyMode();
    
    for ( auto & g : grid ){
        g.color.set(GEL_COLORS[ (int) floor(ofRandom(3)) ] );
        g.color.a = 0;
    }
}

//--------------------------------------------------------------
void GridMeter::editArrows(){
    bWaitingForArrowStartClick = true;
    bWaitingForArrowEndClick = false;
}

//--------------------------------------------------------------
void GridMeter::calcArrowOuterEndpoints(ofVec2f pos){

    // set left arrow outer endpoints
    arrows[0].pointB.set(pos);
    arrows[0].pointF.set(pos.x, getProjectorHeight() - pos.y);

    // set top arrow outer endpoints -- the top is more funky since it's
    // rotated 90º instead of mirrored like the left/right arrows are
    // make a vector from the left arrow point to the end
    ofVec2f arrowVec = pos - arrows[0].pointA;
    arrowVec.rotate(90);

    arrows[1].pointB = arrows[1].pointA + arrowVec;
    arrows[1].pointF.set( getProjectorWidth() - arrows[1].pointB.x, arrows[1].pointB.y);

    // set right arrow outer epoints
    arrows[2].pointB.set(getProjectorWidth() - pos.x, pos.y);
    arrows[2].pointF.set(getProjectorWidth() - pos.x, getProjectorHeight() - pos.y);

}

//--------------------------------------------------------------
void GridMeter::calcArrowInnerEndpoints(){
    // set left arrow inner endpoints
    arrows[0].pointC = arrows[0].pointB + ofPoint(0, Params::level2arrowThickness);
    arrows[0].pointE = arrows[0].pointF - ofPoint(0, Params::level2arrowThickness);

    // set top arrow inner endpoints
    arrows[1].pointC = arrows[1].pointB - ofPoint(Params::level2arrowThickness, 0);
    arrows[1].pointE = arrows[1].pointF + ofPoint(Params::level2arrowThickness, 0);

    // set right arrow inner endpoints
    arrows[2].pointC = arrows[2].pointB + ofPoint(0, Params::level2arrowThickness);
    arrows[2].pointE = arrows[2].pointF - ofPoint(0, Params::level2arrowThickness);
}

//--------------------------------------------------------------
void GridMeter::calcArrowThickness(){
    for (auto& arrow : arrows) arrow.calcInnerPoint();
}

//--------------------------------------------------------------
void GridMeter::mousePressed(ofMouseEventArgs& args){

    if (bWaitingForArrowStartClick) {
        bWaitingForArrowStartClick = false;
        bWaitingForArrowEndClick = true;
    } else if (bWaitingForArrowEndClick) {
        bWaitingForArrowEndClick = false;

        cout << "left arrow"     << endl;
        cout << arrows[0].pointA << endl;
        cout << arrows[0].pointB << endl;
        cout << arrows[0].pointF << endl;

        cout << "top arrow"      << endl;
        cout << arrows[1].pointA << endl;
        cout << arrows[1].pointB << endl;
        cout << arrows[1].pointF << endl;

        cout << "right arrow"    << endl;
        cout << arrows[2].pointA << endl;
        cout << arrows[2].pointB << endl;
        cout << arrows[2].pointF << endl;
    }

}

//--------------------------------------------------------------
void GridMeter::mouseMoved(ofMouseEventArgs& args){

//    pulses[0].shape.setX(args.x - pulses[0].shape.getWidth()/2);
//    pulses[0].shape.setY(args.y - pulses[0].shape.getHeight()/2);

    if (bWaitingForArrowStartClick) {
        // point of the left arrow is mouse x
        arrows[0].pointA.set(args.x, getProjectorHeight()/2.);

        // point of top arrow is mouse distance from left edge
        arrows[1].pointA.set(getProjectorWidth()/2., args.x);

        // set the point of the right arrow to the mouse x
        arrows[2].pointA.set(getProjectorWidth() - args.x, getProjectorHeight()/2.);
    } else if (bWaitingForArrowEndClick) {

        calcArrowOuterEndpoints(args);
        calcArrowInnerEndpoints();
        calcArrowThickness();

    }

}

//--------------------------------------------------------------
void GridMeter::mouseReleased(ofMouseEventArgs& args){
}

//--------------------------------------------------------------
void GridMeter::mouseDragged(ofMouseEventArgs& args){
    
}

#pragma mark Arrow

//--------------------------------------------------------------
void Arrow::calcInnerPoint(){
    ofVec2f upperVec = pointC - pointB + pointA;
    ofVec2f lowerVec = pointE - pointF + pointA;
    ofLineSegmentIntersection(pointC, upperVec, pointE, lowerVec, pointD);

    shape.clear();
    shape.addVertex(pointA);
    shape.addVertex(pointB);
    shape.addVertex(pointC);
    shape.addVertex(pointD);
    shape.addVertex(pointE);
    shape.addVertex(pointF);
    shape.close();
}
