//
//  particle.cpp
//  wpaFloor
//
//  Created by t3rr0r on 15/04/2019.
//

#include "particle.hpp"

//------------------------------------------------------------------
Particle::Particle(ofParameterGroup & particleParameters){
    attractPoints = NULL;
    parameters = particleParameters;
}

//------------------------------------------------------------------
void Particle::setAttractPoints( vector <ofPoint> * attract ){
    attractPoints = attract;
}

//------------------------------------------------------------------
void Particle::reset(int peerId){
    //the unique val allows us to set properties slightly differently for each particle
    uniqueVal = ofRandom(-10000, 10000);
    id = peerId;
    
    pos.x = ofRandomWidth();
    pos.y = ofRandomHeight();
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    
    frc   = ofPoint(0,0,0);
    
    scale = ofRandom(0.5, 1.0);
    
    drag  = ofRandom(parameters.getFloat("dragMin"), parameters.getFloat("dragMax"));
}

//------------------------------------------------------------------
void Particle::update(){
    vel *= drag;

    if( attractPoints ){
        
        //1 - find closest attractPoint
        ofPoint closestPt;
        int closest = -1;
        float closestDist = 9999999;
        
        for(unsigned int i = 0; i < attractPoints->size(); i++){
            float lenSq = ( attractPoints->at(i)-pos ).lengthSquared();
            if( lenSq < closestDist ){
                closestDist = lenSq;
                closest = i;
            }
        }
        
        //2 - if we have a closest point - lets calcuate the force towards it
        if( closest != -1 ){
            closestPt = attractPoints->at(closest);
            float dist = sqrt(closestDist);
            
            //in this case we don't normalize as we want to have the force proportional to distance
            frc = closestPt - pos;
            
            //lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
            if( dist > parameters.getInt("gravity distance") && !ofGetKeyPressed('f')){
                vel += frc * parameters.getFloat("gravity");
            } else {
                //if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
                frc.x = ofSignedNoise(uniqueVal, pos.y * parameters.getFloat("noise position"), ofGetElapsedTimef() * parameters.getFloat("noise time"));
                frc.y = ofSignedNoise(uniqueVal, pos.x * parameters.getFloat("noise position"), ofGetElapsedTimef() * parameters.getFloat("noise time"));
                vel += frc * parameters.getFloat("noise force");
            }
            
        }
        
    }
    
    ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
    frc = attractPt-pos;
    
    //let get the distance and only repel points close to the mouse
    float dist = frc.length();
    frc.normalize();
    
    if( dist < 150 ){
        vel += -frc * 3.5; //notice the frc is negative
    }
    
    // UPDATE OUR POSITION
    pos += vel;
    
    
    // (optional) LIMIT THE PARTICLES TO STAY ON SCREEN
    //we could also pass in bounds to check - or alternatively do this at the ofApp level
    if( pos.x > ofGetWidth() ){
        pos.x = ofGetWidth();
        vel.x *= -1.0;
    }else if( pos.x < 0 ){
        pos.x = 0;
        vel.x *= -1.0;
    }
    if( pos.y > ofGetHeight() ){
        pos.y = ofGetHeight();
        vel.y *= -1.0;
    }
    else if( pos.y < 0 ){
        pos.y = 0;
        vel.y *= -1.0;
    }
    
}

void Particle::findPeer(vector <Particle> & allPeers) {
    for (unsigned int i = 0; i < allPeers.size(); i++) {
        Particle peer = allPeers[i];
        if(std::find(peerIds.begin(), peerIds.end(), i) != peerIds.end()) {
            continue;
        }
        
        float distance = (pos - peer.pos).length();
        if (distance < 150 && peer.peerIds.size() < parameters.getInt("connections")) {
            peerIds.push_back(i);
            allPeers[i].peerIds.push_back(id);
            return;
        }
    }
}

void Particle::updatePeers(vector <Particle> & allPeers){
    for (unsigned int i = 0; i < peerIds.size(); i++) {
        int peerId = peerIds[i];
        Particle peer = allPeers[peerId];
        float distance = (pos - peer.pos).length();
        if (distance > 150) {
            peerIds.erase(std::remove(peerIds.begin(), peerIds.end(), peerId), peerIds.end());
            allPeers[peerId].peerIds.erase(std::remove(allPeers[peerId].peerIds.begin(), allPeers[peerId].peerIds.end(), id), allPeers[peerId].peerIds.end());
            findPeer(allPeers);
        }
    }
    if (peerIds.size() < parameters.getInt("connections")) {
        for (unsigned int i = peerIds.size(); i < parameters.getInt("connections"); i++) {
            findPeer(allPeers);
        }
    }
}

//------------------------------------------------------------------
void Particle::draw(vector <Particle> & allPeers){
    ofSetColor(parameters.getColor("color"));
    ofDrawCircle(pos.x, pos.y, scale * 4.0);
    
    for (unsigned int i = 0; i < peerIds.size(); i++) {
        int peerId = peerIds[i];
        ofPoint peerPos = allPeers[peerId].pos;
        ofSetColor(parameters.getColor("color"));
        ofDrawLine(pos.x, pos.y, peerPos.x, peerPos.y);
    }
}
