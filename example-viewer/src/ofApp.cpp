#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetVerticalSync(false);

	cam.connect();
	cam.openStream();

	ofLog() << "Wait...";

	ofSleepMillis(1000);

	cam.setGamma(1.0);
	cam.setBrightness(100);
	cam.setFPS(800);

	screen.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {
	int cout = 0;;
	while (cam.frameBuffer.tryReceive(frameMat))
	{
		cout++;
	}

	if (frameCount.size() > 100)
		frameCount.erase(frameCount.begin());

	frameCount.push_back(cout);
}

//--------------------------------------------------------------
void ofApp::exit() {
	cam.stopStream();
	cam.disConnect();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if ((ofGetElapsedTimeMillis() - timer) > (1000 / 60)) {
		screen.begin();
		ofClear(0);
		ofxCv::drawMat(frameMat, 0, 0, screen.getWidth(), screen.getHeight());

		ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, 20);
		ofDrawBitmapStringHighlight(ofToString(cam.getFPS()), 20, 40);
		ofDrawBitmapStringHighlight(ofToString(cam.getFrameID()), 20, 60);

		timer = ofGetElapsedTimeMillis();
		screen.end();
	}

	screen.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	screen.allocate(w, h);
}
