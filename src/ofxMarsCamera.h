#pragma once

#include "ofxOpenCv.h"

#include "define.h"
#include "IntTypes.h"
#include "SDK.h"

class MarsCamera : public ofThread {

public:

	MarsCamera() {
		isConnected = false;
	}

	ofThreadChannel<cv::Mat> frameBuffer;

	void connect();
	void openStream();
	void stopStream();
	void disConnect();
	bool update(GENICAM_Frame* frame, cv::Mat& frame_cv);

	float getFPS();
	float getFrameID();

	void setGamma(double gamma);
	void setShutterTime(double shutterTime);
	void setFPS(int fps);
	void setBrightness(int brightness);

private:
	GENICAM_Camera* camera;                 // カメラのインスタンスのポインタ、
	GENICAM_System* system;                 // カメラの接続数の取得、接続、切断を司るシステムのインスタンス
	GENICAM_Camera* cameraList;             // APIでは接続してるカメラ全てが返ってくる
	GENICAM_StreamSource* streamSource;     // カメラからのデータ
	GENICAM_ISPControl* ispControl;         // ~Controlは各種設定、cameraインスタンスから作る
	GENICAM_AnalogControl* analogControl;
	GENICAM_AcquisitionControl* acquisitionControl;
	GENICAM_ImageFormatControl* imageFormarControl;
	GENICAM_UserSetControl* userSetControl;

	int32_t isError;

	bool isConnected;
	bool bStreaming;

	ofFpsCounter fps;
	int frameID;

	int imageWidth;
	int imageHeight;
	GENICAM_Frame* frame;
	unsigned char* imageBuffer;
	cv::Mat frameMat;

	void threadedFunction();
	void connectCamera();
	void createControls();
	void startCapture();
	void stopCapture();
	void disConnectCamera();
	void getWidthFromSystem();
	void getHeightFromSystem();

};