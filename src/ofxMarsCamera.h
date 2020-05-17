#pragma once

#include <opencv2/opencv.hpp>
#include "../Dependencies/IntTypes.h"
#include "../Dependencies/SDK.h"

using namespace ofxCv;
using namespace cv;

class MarsCamera: public ofThread {
    
    public:
        ofImage cameraImage; // カメラの画像
        void setThreadExecuteTime(int time); // setThered~ -> connect -> setCamera~ -> openStreamの順なら問題ないと思う
        void connect();
        void setCameraValues(
            double gamma, 
            double gain, 
            double shutterTime,
            int brightness,
            int contrast
        );
        void openStream();
        void stopStream();
        void disConnect();

    private:
        ofTimer timer;                          // スレッドの実行時間
        GENICAM_Camera *camera;                 // カメラのインスタンスのポインタ、
        GENICAM_System *system;                 // カメラの接続数の取得、接続、切断を司るシステムのインスタンス
        GENICAM_Camera *cameraList;             // APIでは接続してるカメラ全てが返ってくる
        GENICAM_StreamSource *streamSource;     // カメラからのデータ
        GENICAM_ISPControl *ispControl;         // ~Controlは各種設定、cameraインスタンスから作る
        GENICAM_AnalogControl *analogControl;
        GENICAM_AcquisitionControl *acquisitionControl;
        GENICAM_ImageFormatControl *imageFormarControl;
        GENICAM_UserSetControl *userSetControl;

        void setGamma(double gamma);
        void setGain(double gain);
        void setShutterTime(double shutterTime);
        void setBrightness(int brightness);
        void setContrast(int contrast);
        void connectCamera();
        void createControls();
        void getWidthFromSystem();
        void getHeightFromSystem();
        void startCapture();
        void stopCapture();
        void disConnectCamera();
};
