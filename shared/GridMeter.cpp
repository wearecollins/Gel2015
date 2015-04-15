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
    color = ofColor(255, 150);
    
    ofPushMatrix();
    ofPushStyle();
    ofFill();
    ofSetColor(color);
    ofCircle(*this, radius);
    
//    ofSetColor(255);
//    ofNoFill();
//    ofCircle(*this, radius);
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void GridPoint::activate( float level ){
    color.a = level;
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

    arrows.resize(3);
    bWaitingForArrowStartClick = false;
    bWaitingForArrowEndClick = false;
}

//--------------------------------------------------------------
void GridMeter::setupGrid(){

    // grid of 300?
    float space = 10;
    float w = getProjectorWidth() - space * 5.;
    float h = getProjectorHeight() - space * 5.;
    
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
            grid.back().color.setHue(ofMap(x, 0, Params::level2gridStepsX, 0, 125) + ofMap(y, 0, Params::level2gridStepsY, 0, 130));
        }
    }
}

//--------------------------------------------------------------
void GridMeter::render(){
    renderTexture.begin();
    ofClear(255);
    ofPushMatrix();
    
    // activate based on value
    ofVec2f pnt = getGridPoint(value);
    
    // radius of "hit" based on grid point
    float rad = 300;
    
    if ( messages != NULL ){
        for (auto & g : grid ){
            for ( auto & m : *messages ){
                ofVec2f pnt2 = getGridPoint(m.second.direction);
                if ( abs( g.distance(pnt2)) < rad/4. && bActive ){
                    g.activate(50);
                }
            }
        }
    }
    
    static bool blogged = false;
    
    for (auto & g : grid ){
        if ( bActive && !blogged ){
//            cout << abs( g.distance(pnt)) << endl;
        }
        if ( abs( g.distance(pnt)) < rad && bActive ){
            g.activate();
        }
        g.draw();
    }
    
    if ( !blogged && bActive){
        blogged = true;
    }


    // for "editing" the arrow endpoints
    ofPushStyle();
    ofSetColor(ofColor::cyan);

    if (bWaitingForArrowStartClick) {
        ofFill();
        ofCircle(arrows[0].start, 15);  // point of left arrow where mouse is
        ofCircle(arrows[1].start, 15);  // point of right arrow is mirrored mouse point
        ofCircle(arrows[2].start, 15);  // point of top arrow is mouse distance from left edge
    } else {
        ofSetLineWidth(10);

        // left arrow
        ofLine(arrows[0].start, arrows[0].end);
        ofLine(arrows[0].start, arrows[0].otherEnd);

        // right arrow
        ofLine(arrows[2].start, arrows[2].end);
        ofLine(arrows[2].start, arrows[2].otherEnd);

        // top arrow
        ofLine(arrows[1].start, arrows[1].end);
        ofLine(arrows[1].start, arrows[1].otherEnd);
    }

    ofPopStyle();

    ofPopMatrix();
    renderTexture.end();
    
    ofSetColor(255, fill.a);
    
    renderTexture.draw(0,0);
}

//--------------------------------------------------------------
void GridMeter::partyMode(){
    
}

//--------------------------------------------------------------
void GridMeter::editArrows(){
    bWaitingForArrowStartClick = true;
    bWaitingForArrowEndClick = false;
}

//--------------------------------------------------------------
void GridMeter::mousePressed(ofMouseEventArgs& args){

    if (bWaitingForArrowStartClick) {
        bWaitingForArrowStartClick = false;
        bWaitingForArrowEndClick = true;
    } else if (bWaitingForArrowEndClick) {
        bWaitingForArrowEndClick = false;
    }

}

//--------------------------------------------------------------
void GridMeter::mouseMoved(ofMouseEventArgs& args){

    if (bWaitingForArrowStartClick) {
        // point of the left arrow is mouse x
        arrows[0].start.set(args.x, getProjectorHeight()/2.);

        // point of top arrow is mouse distance from left edge
        arrows[1].start.set(getProjectorWidth()/2., args.x);

        // set the point of the right arrow to the mouse x
        arrows[2].start.set(getProjectorWidth() - args.x, getProjectorHeight()/2.);
    } else if (bWaitingForArrowEndClick) {

        // make a vector from the left arrow point to the end
        // (the end is your mouse cursor)
        ofVec2f arrowVec = args - arrows[0].start;
        arrowVec.rotate(90);

        // set left arrow endpoint
        arrows[0].end.set(args);
        arrows[0].otherEnd.set(args.x, getProjectorHeight() - args.y);

        // set top arrow endpoint
        arrows[1].end = arrows[1].start + arrowVec;
        arrows[1].otherEnd.set( getProjectorWidth() - arrows[1].end.x, arrows[1].end.y);

        // set right arrow endpoint
        arrows[2].end.set(getProjectorWidth() - args.x, args.y);
        arrows[2].otherEnd.set(getProjectorWidth() - args.x, getProjectorHeight() - args.y);

    }

}

//--------------------------------------------------------------
void GridMeter::mouseReleased(ofMouseEventArgs& args){
}

//--------------------------------------------------------------
void GridMeter::mouseDragged(ofMouseEventArgs& args){
    
}