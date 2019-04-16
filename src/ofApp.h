#pragma once

#include "ofMain.h"
#include "particle.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:
  void setup();
  void update();
  void draw();
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

  bool bHide;

  ofParameter <float> dragMin;
  ofParameter <float> dragMax;
  ofParameter <float> noisePosition;
  ofParameter <float> noiseTime;
  ofParameter <float> noiseForce;
  ofParameter <int> connections;
  ofParameter <float> gravity;
  ofParameter <int> gravityDistance;
  ofParameter <float> attractFactor;
  ofParameter <ofColor> color;
  ofParameterGroup parameters;

  vector <Particle> p;
  vector <ofPoint> attractPoints;
  vector <ofPoint> attractPointsWithMovement;
  ofxIntSlider radius;
  ofxPanel gui;
};
