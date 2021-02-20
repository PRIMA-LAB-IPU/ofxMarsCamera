#pragma once

#include "ofMain.h"
#include "ofxMarsCamera.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void windowResized(int w, int h);

		cv::Mat frameMat;
		MarsCamera cam;
		
		ofFbo screen;
		float timer;
		vector<int> frameCount;
};
