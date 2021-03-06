//
//  particle.hpp
//  wpaFloor
//
//  Created by mistakia on 15/04/2019.
//

#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>

#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxColorGradient.h"

class Particle{

public:
    Particle(ofParameterGroup & particleParameters);

    void setAttractPoints( vector <ofPoint> * attract );

    void reset(int peerId);
    void update();
    void updateContours(ofxCvContourFinder & contourFinder, int offsetX, int offsetY);
    void updatePeers(vector <Particle> & allPeers);
    void findPeer(vector <Particle> & allPeers);
    void draw(vector <Particle> & allPeers);

    ofParameterGroup parameters;

    ofPoint pos;
    ofPoint vel;
    ofPoint frc;

    int id;
    float drag;
    float uniqueVal;
    ofColor color;
    float scale;

    vector <ofPoint> * attractPoints;
    vector <int> peerIds;
};

#endif /* particle_hpp */
