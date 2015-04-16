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

    colors.push_back(ofColor::yellow);
    colors.push_back(ofColor::cyan);
    colors.push_back(ofColor::magenta);
    
    vector<ofMesh> tempLiveMesh;
    Svg2Mesh::addSvgToMeshVector("graphics/level1/live_view.svg", tempLiveMesh);
    
    liveViewMeshes.push_back(vector<ofMesh>());
    liveViewMeshes.push_back(vector<ofMesh>());
    liveViewMeshes.push_back(vector<ofMesh>());
    
    for ( auto & m : tempLiveMesh ){
        auto c = m.getColor(0);
        ofFloatColor color;
        
        if ( c == ofFloatColor::green ){
            liveViewMeshes[1].push_back(m);
            for ( auto & c : liveViewMeshes[1].back().getColors() ){
                c.set(ofFloatColor::cyan);
            }
            
        } else if ( c == ofFloatColor::red ){
            liveViewMeshes[0].push_back(m);
            for ( auto & c : liveViewMeshes[0].back().getColors() ){
                c.set(ofFloatColor::yellow);
            }

        } else if ( c == ofFloatColor::blue ){
            liveViewMeshes[2].push_back(m);
            
            for ( auto & c : liveViewMeshes[2].back().getColors() ){
                c.set(ofFloatColor::magenta);
            }
        }
        
    }
    
    
    
    
//    liveViewMesh.push_back(ofMesh());
//    liveViewMesh.push_back(ofMesh());
//    liveViewMesh.push_back(ofMesh());
//    
//    int i1 = 0;
//    int i2 = 0;
//    int i3 = 0;
//    
//    for ( auto & i : tempLiveMesh.getIndices() ){
//        auto v = tempLiveMesh.getVertex(i);
//        auto c = tempLiveMesh.getColor(i);
//        if ( c == ofFloatColor::green ){
//            liveViewMesh[1].addColor(ofFloatColor(255.,0));
//            liveViewMesh[1].addVertex(v);
//            liveViewMesh[1].addIndex(i1);
//            i1++;
//            
//        } else if ( c == ofFloatColor::red ){
//            liveViewMesh[0].addColor(ofFloatColor(255.,0));
//            liveViewMesh[0].addVertex(v);
//            liveViewMesh[0].addIndex(i2);
//            i2++;
//            
//        } else if ( c == ofFloatColor::blue ){
//            liveViewMesh[2].addColor(ofFloatColor(255.,0));
//            liveViewMesh[2].addVertex(v);
//            liveViewMesh[2].addIndex(i3);
//            i3++;
//            
//        }
//    }
    
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
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), colors[i]);
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310, colors[i]) ); // innermost line
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925, colors[i]) );     // outermost line

    // middle segment
    i++;
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), colors[i]);
    segments[i].meshes.push_back( createMesh(lines[i], 4540, 5321, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 3682, 4500, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 2802, 3648, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 1893, 2766, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 962, 1857, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 922, colors[i]) );

    // right segment
    i++;
    segments[i].fullMesh = createMesh(lines[i], 0, lines[i].getVertices().size(), colors[i]);
    segments[i].meshes.push_back( createMesh(lines[i], 4525, 5310, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 3675, 4484, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 2797, 3633, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 1898, 2760, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 963, 1856, colors[i]) );
    segments[i].meshes.push_back( createMesh(lines[i], 0, 925, colors[i]) );

}

//--------------------------------------------------------------
ofMesh Meter::createMesh(ofPolyline &line, int lowerBound, int higherBound, ofColor color){

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    vector<ofPoint>& vertices = line.getVertices();

    for (int i = lowerBound; i < higherBound; i++) {
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
void Meter::createBlip(string name, float value){
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
    
    if ( blips.count(name) == 0 ){
//        blips[name] = MeshBlip();
    } else if ( blips[name]->vertexIndex == segmentIndex){
        return;
    } else if ( blips[name]->vertexIndex != segmentIndex){
        auto * kill = blips[name];
        blips.erase(name);
        delete kill;
    }
    MeshBlip * blip = new MeshBlip();
    
    auto & ranMesh = liveViewMeshes[segmentIndex][floor(ofRandom(0, liveViewMeshes[segmentIndex].size()))];
//    blip.vertexIndex = floor(ofRandom(0, 111));
    blip->vertexIndex = segmentIndex;
    blip->parent = &ranMesh;
    blip->animation = new ofxAnimatableFloat();
    
    blip->animation->reset(0.0);
    blip->animation->setCurve(BLINK_AND_FADE_1);
    blip->animation->setDuration(0.33);
    blip->animation->animateTo(1.);
    ofAddListener(blip->animation->animFinished, this, &Meter::onBlipFinished);
    
    blips[name] = blip;
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
void Meter::onBlipFinished(ofxAnimatable::AnimationEvent& args){
//    for (auto & it : blips ){
//        if ( args.who == it.second->animation ){
//            blips.erase(it.first);
//        }
//    }
}

//--------------------------------------------------------------
void Meter::render(){
    ofPushStyle();

    if (bActive) {
        createPulse(value);
    }
    
    // @MATT: IT WOULD BE SUPER HELPFUL TO SEE SOME LIGHT FEEDBACK
    // FROM ALL INPUT; IS THERE A WAY TO MAKE A "LIGHT" FEEDBACK?
    
    if (bActive) {
        if ( messages != NULL ){
            for ( auto & m : *messages ){
                createBlip(m.uniqueId, m.direction);
            }
        }
    }

    // update animatables
    for (auto& segment : segments) {
        for (int i = 0; i < segment.pulses.size(); i++) {
            segment.pulses[i]->update(1.0 / 60.0);
        }
    }


//    renderTexture.begin();
    ofSetColor(255,10);
    ofRect(0, 0, renderTexture.getWidth(), renderTexture.getHeight());

    ofSetColor(ofColor::white, 50);
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
        
        for (auto& it : blips) {
            it.second->animation->update(1.0 / 60.0);
            if ( it.second->animation->hasFinishedAnimating() ){
                blips.erase(it.first);
            }
        }
        
        for (auto& it : blips) {
            for ( auto & c : it.second->parent->getColors() ){
                c.set(c.r,c.g,c.b, it.second->animation->val());
            }
        }
        
        for ( auto & g : liveViewMeshes ){
            for ( auto & m : g ){
                m.draw();
                for ( auto & c : m.getColors() ){
                    c.set(c.r,c.g,c.b,0.);
                }
            }
        }
        
    } else {
        // draw the meshes
        for (auto& rainbowSegment : segments) {

            vector<ofFloatColor>& vertColors = rainbowSegment.fullMesh.getColors();

            PartySegment lastSegment;
            float offset = ofWrap(ofGetFrameNum()*5, 0, vertColors.size());

            for (auto& partySegment : partyModeSegments) {

                int length = partySegment.startingIndex - lastSegment.startingIndex;

                for (int i = 0; i < length; i++) {

                    // instead of a liner 0 -> 1 for the alpha values, let's shape
                    // the curve a little bit so we get more color & less white
                    float alpha = (float) i / length; // linear 0 -> 1
                    alpha = 1 - alpha; // invert it!
                    alpha = powf(alpha, 2); // square it!
                    alpha = 1 - alpha; // back to normal, but squared!

                    int index = ofWrap(i + partySegment.startingIndex + offset, 0, vertColors.size());
                    vertColors[index].set(partySegment.color);
                    vertColors[index].a = alpha;
                }

                lastSegment = partySegment;

            }

            rainbowSegment.fullMesh.drawWireframe();
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

    
    ofPopStyle();
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
void Meter::partyMode(){
    LiveInput::partyMode();

    // chop the full line into a bunch of smaller segments of varying length
    // and sequential colors

    int colorIndex = 0;
    int segmentStartingIndex = 0;
    partyModeSegments.clear();

    while (segmentStartingIndex <= segments[0].fullMesh.getVertices().size()) {
        int segmentLength = (int) ofRandom(50, 150);
        segmentStartingIndex += segmentLength;
        colorIndex = ++colorIndex % colors.size();

        PartySegment ps;
        ps.startingIndex = segmentStartingIndex;
        ps.color = colors[colorIndex];
        partyModeSegments.push_back(ps);
    }

}
