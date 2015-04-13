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
    meter.loadImage("graphics/level1/level1_meter_no_background.png");
    renderShader.load("","shaders/meterRender.frag");
    svg.load("graphics/level1/level1_meter.svg");

    segments.resize(3);

    // load full line paths from SVG file
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath& path = svg.getPathAt(i);
        ofPolyline outline = path.getOutline()[0].getResampledBySpacing(1);
        lines.push_back( outline );
    }

    // chop each line into parts between the corners and make a mesh for each

    int i = 0;

    // left segment
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), ofColor::yellow);
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310, ofColor::yellow) ); // innermost line
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484, ofColor::yellow) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633, ofColor::yellow) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760, ofColor::yellow) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856, ofColor::yellow) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925, ofColor::yellow) );     // outermost line

    // middle segment
    i++;
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), ofColor::cyan);
    segments[i].meshes.push_back( createMesh(lines[i], 4540, 5321, ofColor::cyan) );
    segments[i].meshes.push_back( createMesh(lines[i], 3682, 4500, ofColor::cyan) );
    segments[i].meshes.push_back( createMesh(lines[i], 2802, 3648, ofColor::cyan) );
    segments[i].meshes.push_back( createMesh(lines[i], 1893, 2766, ofColor::cyan) );
    segments[i].meshes.push_back( createMesh(lines[i], 962, 1857, ofColor::cyan) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 922, ofColor::cyan) );

    // right segment
    i++;
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), ofColor::magenta);
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310, ofColor::magenta) );
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484, ofColor::magenta) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633, ofColor::magenta) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760, ofColor::magenta) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856, ofColor::magenta) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925, ofColor::magenta) );

}

//--------------------------------------------------------------
ofMesh Meter::createMesh(ofPolyline &line, int lowerBound, int higherBound, ofColor color){

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    vector<ofPoint>& vertices = line.getVertices();

    for (int i = lowerBound; i <= higherBound; i++) {
        if (vertices[i] == ofVec3f::zero()) continue;
        mesh.addVertex(vertices[i]);
        mesh.addColor(color);
    }

    return mesh;
    
}

//--------------------------------------------------------------
float Meter::calcAlphaForPulse(float percent, int index, int numLines, float overlap){

    // calculates an alpha value for a given pulse moving through the
    // radiating lines of the rainbow segment

    //      -->  pulse -->
    //         |-------|
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //   |   |   ||  ||  |   |
    //
    //   1   2   3   4   5   6


    float duration = (float) 1.0 / numLines;
    float offset = index * duration;

    float startPoint = MAX(0, offset - overlap);
    float endPoint = MIN(1, offset + overlap + duration);

    float alpha;

    // hasn't started yet
    if (percent < startPoint)
        alpha = 0;
    // pulse is ramping up (between start & end)
    else if (percent > startPoint && percent < endPoint)
        alpha = ofMap(percent, startPoint, endPoint, 0.0f, 1.0f, true);
    // pulse is fading out (between end & end + duration)
    else if (percent > endPoint && percent < endPoint + duration)
        alpha = ofMap(percent, endPoint, endPoint + duration, 1.0f, 0.0f, true);
    // pulse has passed
    else
        alpha = 0;
    
    return alpha;
    
}

//--------------------------------------------------------------
void Meter::createPulse(float value){

    Poco::LocalDateTime now;
    Poco::Timespan timediff;
    int segmentIndex;

    // figure out which rainbow segment the pulse should correspond to
    if (value < 0.5)
        segmentIndex = 0;
    else if (value >= 0.5 && value < 1.5)
        segmentIndex = 1;
    else if (value > 0.5)
        segmentIndex = 2;

    // make sure enough time has passed before creating a new pulse
    timediff = now - segments[segmentIndex].lastPulse;
    if (!( (float) timediff.milliseconds() / 1000.0f > 0.3 )) return;

    // create a new pulse
    ofxAnimatableFloat* anim = new ofxAnimatableFloat();
    anim->reset(-0.3);
    anim->setCurve(LINEAR);
    anim->setDuration(0.33);
    anim->animateTo(1.3);

    segments[segmentIndex].lastPulse = now;
    segments[segmentIndex].pulses.push_back(anim);
    ofAddListener(anim->animFinished, this, &Meter::onAnimFinished);
    
}

//--------------------------------------------------------------
void Meter::onAnimFinished(ofxAnimatable::AnimationEvent &args){

    // remove finished pulses
    for (auto& segment : segments) {
        for (int i = 0; i < segment.pulses.size(); i++) {
            if (args.who == segment.pulses[i]) {
                segment.pulses.erase(segment.pulses.begin()+i);
                break;
            }
        }
    }
    
}

//--------------------------------------------------------------
void Meter::render(){

    if (bActive) {
        createPulse(value);
    }
    
    // @MATT: IT WOULD BE SUPER HELPFUL TO SEE SOME LIGHT FEEDBACK
    // FROM ALL INPUT; IS THERE A WAY TO MAKE A "LIGHT" FEEDBACK?
    
//    if ( messages != NULL ){
//        for ( auto & m : *messages ){
//            createPulse(m.second.direction);
//        }
//    }

    // update animatables
    for (auto& segment : segments) {
        for (int i = 0; i < segment.pulses.size(); i++) {
            segment.pulses[i]->update(1.0 / 60.0);
        }
    }


//    renderTexture.begin();
    ofSetColor(255,10);
    ofRect(0, 0, renderTexture.getWidth(), renderTexture.getHeight());

    ofSetColor(ofColor::white);
    meter.draw(0, 0);

    // draw the full line
    ofNoFill();
    ofSetLineWidth(10);
    ofSetColor(ofColor::white);
    
//    for (auto& line : lines){
//
//        ofBeginShape();
//        for (int i = 0; i < line.size(); i++){
//            ofVertex(line[i]);
//        }
//        ofEndShape();
//    }

    if (!bPartyMode) {
        // draw the meshes
        for (auto& segment : segments) {
            // loop through each mesh of the segment
            // (meshes correspond to each radiating line)
            for (int i = 0; i < segment.meshes.size(); i++) {

                // reset alpha back to 0 for all
                for (auto& color : segment.meshes[i].getColors()) {
                    color.a = 0;
                }

                // loop through each active pulse and accumulate alpha for each
                // line of the segment
                for (auto& pulse : segment.pulses) {
                    float alpha = calcAlphaForPulse(pulse->val(), i, segment.meshes.size(), 0.2);
                    for (auto& color : segment.meshes[i].getColors()) {
                        color.a += alpha;
                    }
                }

                segment.meshes[i].drawWireframe();
            }
        }
    } else {
        // draw the meshes
        for (auto& segment : segments) {

            vector<ofFloatColor>& colors = segment.fullMesh.getColors();

            // create an offset that changes with time
            float j = ofGetElapsedTimef() * 3;

            for (int i = 0; i < colors.size(); i++) {
                // number of steps it takes to go from 0 -> 100% alpha
                j += 1. / 150.;
                j = fmod(j, 1);

                colors[i].a = j;
            }

            segment.fullMesh.drawWireframe();
        }
    }

    // helper to find the indices in the polyline for where each radiating
    // line begins & ends
//    if (ofGetKeyPressed(OF_KEY_SHIFT))
//        drawClosestPoint();

//    renderTexture.end();
//
////    ofSetColor(ofColor::white);
//    ofSetColor(255, fill.a);
//
//    renderShader.begin();
//    renderShader.setUniformTexture("alpha", meter.getTextureReference(), 2);
//    renderShader.setUniformTexture("render", renderTexture.getTextureReference(), 3);
//    renderTexture.draw(0,0 );
//    renderShader.end();

}

//--------------------------------------------------------------
void Meter::drawClosestPoint(){

    float closestDistance = 999999999999;
    int closestIndex;
    ofVec2f closestPoint;

    for (auto& line : lines){
        for (int i = 0; i < line.size(); i++){

            float distSquared = ofDistSquared(ofGetMouseX(), ofGetMouseY(), line[i].x, line[i].y);
            if (distSquared < closestDistance) {
                closestIndex = i;
                closestPoint.set(line[i]);
                closestDistance = distSquared;
            }
        }
    }

    ofDrawBitmapStringHighlight(ofToString(closestIndex), closestPoint + ofVec2f(20, 0));
    ofDrawBitmapStringHighlight(ofToString(closestPoint), closestPoint + ofVec2f(20, 20));

    ofFill();
    ofSetColor(ofColor::magenta);
    ofCircle(closestPoint, 15);
    
}


//--------------------------------------------------------------
//void Meter::partyMode(){
//
//}
