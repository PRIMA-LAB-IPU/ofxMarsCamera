# ofxMarsCamera

### Description

This addon is a driver plug-in for high-speed photography cameras developed by Hangzhou Contrastech Co. 

* Device : [Mars 640-815us](http://www.contrastech.com/en/productview/1033.html)
* Framerate : 1 ~ 816fps
* Support OS : Windows 32/64bit
* Multithread support 
* Support gamma and brightness control

### Dependencies

* ofxOpenCv
* ofxCv

### Compatibility

Tested with: 
* openFrameworks 0.10.x / 0.11.x
* Windows 10 64bit, Visual Studio 2017 / 2019

### Usage

#### ofApp.h

    // ofApp.h
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
        void keyPressed(int key);

        cv::Mat frameMat;
        MarsCamera cam;

        ofFbo screen;
        float timer;
        vector<int> frameCount;
    };

#### ofApp.cpp

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
      // Limit drawing speed
      if ((ofGetElapsedTimeMillis() - timer) > (1000 / 60)) {
        screen.begin();
        ofClear(0);
        cv::drawMat(frameMat, 0, 0, screen.getWidth(), screen.getHeight());

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

### Update

* [2021/02/20] bugfix and add example code
