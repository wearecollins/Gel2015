#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0);
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(1920, 1080);

    mask.loadImage("level1_meter.png");
    svg.load("level1_meter.svg");

    segments.resize(3);

    // load full line paths from SVG file
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath& path = svg.getPathAt(i);
        ofPolyline outline = path.getOutline()[0].getResampledBySpacing(1);

        segments[i].line = outline;
        lines.push_back( outline );
    }

    // chop each line into parts between the corners and make a mesh for each

    int i = 0;

    // left segment
    c = ofColor::yellow;
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310) ); // innermost line
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925) );     // outermost line

    // middle segment
    i++;
    c = ofColor::cyan;
    segments[i].meshes.push_back( createMesh(lines[i], 4540, 5321) );
    segments[i].meshes.push_back( createMesh(lines[i], 3682, 4500) );
    segments[i].meshes.push_back( createMesh(lines[i], 2802, 3648) );
    segments[i].meshes.push_back( createMesh(lines[i], 1893, 2766) );
    segments[i].meshes.push_back( createMesh(lines[i], 962, 1857) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 922) );

    // right segment
    i++;
    c = ofColor::magenta;
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310) );
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925) );

}

//--------------------------------------------------------------
ofMesh ofApp::createMesh(ofPolyline &line, int lowerBound, int higherBound){

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    vector<ofPoint>& vertices = line.getVertices();

    for (int i = lowerBound; i <= higherBound; i++) {
        if (vertices[i] == ofVec3f::zero()) continue;
        mesh.addVertex(vertices[i]);
        mesh.addColor(c);
    }

    return mesh;

}

//--------------------------------------------------------------
float ofApp::calcAlphaForPulse(float percent, int index, int numLines, float overlap){

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

    return alpha;

}

//--------------------------------------------------------------
void ofApp::createPulse(float value){

    value = (int) value;

    // create a new pulse
    ofxAnimatableFloat* anim = new ofxAnimatableFloat();
    anim->reset(-0.3);
    anim->setCurve(LINEAR);
    anim->setDuration(0.33);
    anim->animateTo(1.3);

    segments[value].pulses.push_back(anim);
    ofAddListener(anim->animFinished, this, &ofApp::onAnimFinished);

}

//--------------------------------------------------------------
void ofApp::onAnimFinished(ofxAnimatable::AnimationEvent &args){

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
void ofApp::update(){
    ofSetWindowTitle( ofToString(ofGetFrameRate()) );
}

//--------------------------------------------------------------
void ofApp::draw(){

    // update animatables
    for (auto& segment : segments) {
        for (int i = 0; i < segment.pulses.size(); i++) {
            segment.pulses[i]->update(1.0 / 60.0);
        }
    }

    ofSetColor(ofColor::white);
    mask.draw(0, 0);

    // draw the full line
    ofNoFill();
    ofSetLineWidth(10);
    ofSetColor(ofColor::white);
//
//    for (auto& line : lines){
//
//        ofBeginShape();
//        for (int i = 0; i < line.size(); i++){
//            ofVertex(line[i]);
//        }
//        ofEndShape();
//    }

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

    if (ofGetKeyPressed(OF_KEY_SHIFT))
        drawClosestPoint();

}

//--------------------------------------------------------------
void ofApp::drawClosestPoint(){

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
void ofApp::keyPressed(int key){

    if (key == '1')
        createPulse(0);
    if (key == '2')
        createPulse(1);
    if (key == '3')
        createPulse(2);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
