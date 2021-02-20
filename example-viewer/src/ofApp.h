#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxMarsCamera.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void windowResized(int w, int h);

		cv::Mat frameMat;
		MarsCamera cam;
		
		ofFbo screen;
		float timer;
		vector<int> frameCount;
};
