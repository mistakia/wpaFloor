#pragma once

#include "ofMain.h"
#include "particle.hpp"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    void resetParticles();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void paramsChanged(ofAbstractParameter & parameter);
    void colorChanged(ofColor & color);
    void radiusChanged(int & radius);

    bool bHide;
    bool bThreshWithOpenCV;
    int angle;
    vector <Particle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;

    ofParameter <float> dragMin;
    ofParameter <float> dragMax;
    ofParameter <float> noisePosition;
    ofParameter <float> noiseTime;
    ofParameter <float> noiseForce;
    ofParameter <int> connections;
    ofParameter <int> connectionLength;
    ofParameter <float> gravity;
    ofParameter <int> gravityDistance;
    ofParameter <float> repel;
    ofParameter <int> repelDistance;
    ofParameter <float> attractFactor;
    ofParameter <int> nodeSize;
    ofParameter <ofColor> color;
    ofParameter <ofColor> color2;
    ofParameter <int> radius;
    ofParameterGroup particleParameters;

    ofParameter <int> nearThreshold;
    ofParameter <int> farThreshold;
    ofParameterGroup kinectParameters;

    ofxPanel gui;

    ofxSyphonServer mainOutputSyphonServer;
    ofxKinect kinect;

    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayImageScaled; // scaled
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    ofxCvContourFinder contourFinder;
};
