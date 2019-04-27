#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    mainOutputSyphonServer.setName("WPA Floor Output");

	// enable depth->video image calibration
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
    kinect.open();		// opens first available kinect

    radius.addListener(this, &ofApp::radiusChanged);
    color.addListener(this, &ofApp::colorChanged);
    color2.addListener(this, &ofApp::colorChanged);
    //ofAddListener(particleParameters.parameterChangedE(), this, &ofApp::paramsChanged);

    particleParameters.setName("particles");
    particleParameters.add(dragMin.set("drag min", 0.9));
    particleParameters.add(dragMax.set("drag max", 0.998));
    particleParameters.add(noiseTime.set("noise time", 0.2, 0.0, 1.0));
    particleParameters.add(noisePosition.set("noise position", 0.01, 0.0, 1.0));
    particleParameters.add(noiseForce.set("noise force", 1.03, 0.0, 5.0));
    particleParameters.add(connections.set("connections", 6, 1, 25));
    particleParameters.add(connectionLength.set("connection length", 150, 10, 1000));
    particleParameters.add(gravity.set("gravity", 0.015, 0.0, 0.03));
    particleParameters.add(gravityDistance.set("gravity distance", 100, 1, 300));
    particleParameters.add(repelDistance.set("repel distance", 150, 1, 500));
    particleParameters.add(repel.set("repel force", 3.5, 0.0, 5.0));
    particleParameters.add(attractFactor.set("attract factor", 12.0, 0.0, 500.0));
    particleParameters.add(nodeSize.set("node size", 4, 2, 60));
    particleParameters.add(color.set("color", ofColor::white));
    particleParameters.add(color2.set("color 2", ofColor::white));
    particleParameters.add(radius.set("radius", 240, 10, 600));

    kinectParameters.setName("kinect");
    kinectParameters.add(nearThreshold.set("near threshold", 230, 0, 255));
    kinectParameters.add(farThreshold.set("far threshold", 70, 0, 255));
    kinectParameters.add(contourMax.set("contour max", (kinect.width*kinect.height)/2, 1, (kinect.width*kinect.height)/2));
    kinectParameters.add(contourMin.set("contour min", 20, 1, (kinect.width*kinect.height)/2));

    gui.setup();
    gui.add(particleParameters);
    gui.add(kinectParameters);

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
    grayImageScaled.allocate(ofGetHeight(), ofGetWidth() / 2);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);

	bThreshWithOpenCV = true;

	ofSetFrameRate(60);

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);

    int num = 1100;
    p.assign(num, Particle(particleParameters));
    resetParticles();

    bHide = false;
}

void ofApp::colorChanged(ofColor &color) {
    resetParticles();
}

void ofApp::paramsChanged(ofAbstractParameter &parameter) {
    resetParticles();
}

void ofApp::radiusChanged(int & radius) {
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
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

    kinect.update();

    if(kinect.isFrameNew()) {

      // load grayscale depth image from the kinect source
      grayImage.setFromPixels(kinect.getDepthPixels());

      // we do two thresholds - one for the far plane and one for the near plane
      // we then do a cvAnd to get the pixels which are a union of the two thresholds
      if(bThreshWithOpenCV) {
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(kinectParameters.getInt("near threshold"), true);
        grayThreshFar.threshold(kinectParameters.getInt("far threshold"));
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
      } else {

        // or we do it ourselves - show people how they can work with the pixels
        ofPixels & pix = grayImage.getPixels();
        int numPixels = pix.size();
        for(int i = 0; i < numPixels; i++) {
          if(pix[i] < kinectParameters.getInt("near threshold") && pix[i] > kinectParameters.getInt("far threshold")) {
            pix[i] = 255;
          } else {
            pix[i] = 0;
          }
        }
      }

      // update the cv images
      grayImage.flagImageChanged();
      grayImage.rotate(90, grayImage.getWidth() / 2, grayImage.getHeight() / 2);
      grayImageScaled.scaleIntoMe(grayImage, CV_INTER_CUBIC);

      contourFinder.findContours(grayImageScaled, 10, contourMax, contourMin, false);
	}

        // particle locations updated
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].update(contourFinder);
    }

    // check if peers are within distance
    for (unsigned int i = 0; i < p.size(); i++) {
        p[i].updatePeers(p);
    }

    //lets add a bit of movement to the attract points
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
         attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef()) * particleParameters.getFloat("attract factor");
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef()) * particleParameters.getFloat("attract factor");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);

    //ofSetColor(255, 255, 255);
    if (!bHide) {
        kinect.drawDepth(0, 0, ofGetWidth(), ofGetHeight());
        kinect.draw(0, 0, ofGetWidth(), ofGetHeight());
        grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        contourFinder.draw();
    }

    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw(p);
    }

    mainOutputSyphonServer.publishScreen();

    // auto draw?
    // should the gui control hiding?
    if (!bHide){
        gui.draw();
    }
}

void ofApp::exit(){
  kinect.setCameraTiltAngle(0); // zero the tilt on exit
  kinect.close();
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
    resetParticles();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
