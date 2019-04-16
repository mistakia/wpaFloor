#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    int num = 800;
    p.assign(num, Particle());
    resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    //these are the attraction points used in the forth demo
    attractPoints.clear();
    
    // set attract points around a circle
    int ox = ofGetWidth() * 0.5f;
    int oy = ofGetHeight() * 0.5f;
    int r = 300;
    int points = 100;
    int radians = 360 / points;
    
    for(int i = 0; i < points; i++){
        int x = ox + r * cos(i * radians);
        int y = oy + r * sin(i * radians);
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
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw(p);
    }
    
    ofSetColor(190);    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
