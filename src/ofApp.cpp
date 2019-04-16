#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    //radius.addListener(this, &ofApp::radiusChanged);
    //ofAddListener(parameters.parameterChangedE(), this, &ofApp::paramsChanged);

    parameters.setName("particles");
    parameters.add(dragMin.set("drag min", 0.9));
    parameters.add(dragMax.set("drag max", 0.998));
    parameters.add(noiseTime.set("noise time", 0.2, 0.0, 1.0));
    parameters.add(noisePosition.set("noise position", 0.01, 0.0, 1.0));
    parameters.add(noiseForce.set("noise force", 1.03, 0.0, 5.0));
    parameters.add(connections.set("connections", 6, 1, 25));
    parameters.add(gravity.set("gravity", 0.015, 0.0, 0.03));
    parameters.add(gravityDistance.set("gravity distance", 100, 50, 300));
    parameters.add(attractFactor.set("attract factor", 12.0, 0.0, 500.0));
    parameters.add(color.set("color", ofColor::white));
    gui.setup();
    gui.add(parameters);
    gui.add(radius.setup("radius", 240, 10, 600));

    int num = 800;
    p.assign(num, Particle(parameters));
    resetParticles();

    bHide = false;
}

void ofApp::paramsChanged(ofAbstractParameter &parameter) {
    resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    //these are the attraction points used in the forth demo
    attractPoints.clear();

    // set attract points around a circle
    int ox = ofGetWidth() * 0.5f;
    int oy = ofGetHeight() * 0.5f;
    int points = 100;
    int radians = 360 / points;

    for(int i = 0; i < points; i++){
        int x = ox + radius * cos(i * radians);
        int y = oy + radius * sin(i * radians);
        attractPoints.push_back(ofPoint(x,y));
    }

    attractPointsWithMovement = attractPoints;

    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setAttractPoints(&attractPointsWithMovement);
        p[i].reset(i);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // particle locations updated
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].update();
    }

    // check if peers are within distance
    for (unsigned int i = 0; i < p.size(); i++) {
        p[i].updatePeers(p);
    }

    //lets add a bit of movement to the attract points
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef()) * parameters.getFloat("attract factor");
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef()) * parameters.getFloat("attract factor");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw(p);
    }

    //ofSetColor(190);

    // auto draw?
    // should the gui control hiding?
    if(!bHide){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h'){
        bHide = !bHide;
    }
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
