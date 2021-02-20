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
	cam.setFPS(240);

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
		cv::drawMat(frameMat, 0, 0, screen.getWidth(), screen.getHeight());

		ofPushMatrix();
		ofTranslate(0, ofGetHeight() - 30);
		if (frameCount.size() > 1) {
			for (int i = 0; i < frameCount.size(); i++) {
				float x = ((float)i / (float)frameCount.size()) * (float)screen.getWidth();
				float h = screen.getWidth() / frameCount.size();
				ofDrawRectangle(x, h * -1, h-2, h-2);
				for (int j = 0; j < frameCount[i]; j++) {
					ofDrawRectangle(x, h * -(j + 1), h-2, h-2);
				}
			}
		}
		ofPopMatrix();

		ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, 20);
		ofDrawBitmapStringHighlight(ofToString(cam.getFPS()), 20, 40);
		ofDrawBitmapStringHighlight(ofToString(cam.getFrameID()), 20, 60);

		timer = ofGetElapsedTimeMillis();
		screen.end();
	}

	screen.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	screen.allocate(w, h);
}