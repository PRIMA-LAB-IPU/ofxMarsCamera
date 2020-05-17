#pragma once

#include <opencv2/opencv.hpp>
#include "../Dependencies/IntTypes.h"
#include "../Dependencies/SDK.h"

using namespace cv;

class MarsCamera: public ofThread {
    public: 
        ofImage cameraImage;

        void setThreadExecuteTime(int time) {
            timer.setPeriodEvent(time) //単位はns
        }

        void connect() {
            connectCamera();
            getHeightFromSystem();
            getWidthFromSystem();
            createControls();
        }

        void setCameraValues(
            double gamma, 
            double gain, 
            double shutterTime,
            int brightness,
            int contrast) {
                setGamma(gamma);
                setGain(gain);
                setShutterTime(shutterTime);
                setBrightness(brightness);
                setContrast(contrast);
        }

        void openStream() {
            startCapture();
        }

        void stopStream() {
            stopCapture();
        }

        void disConnect() {
            disConnectCamera();
        }

    private: 
        ofTimer timer;                          // スレッドの実行時
        GENICAM_Camera *camera;                 // カメラのインスタンスのポインタ、
        GENICAM_System *system;                 // カメラの接続数の取得、接続、切断を司るシステムのインスタンス
        GENICAM_Camera *cameraList;             // APIでは接続してるカメラ全てが返ってくる
        GENICAM_StreamSource *streamSource;     // 
        GENICAM_ISPControl *ispControl;         // ~Controlは各種設定、cameraインスタンスから作る
        GENICAM_AnalogControl *analogControl;
        GENICAM_AcquisitionControl *acquisitionControl;
        GENICAM_ImageFormatControl *imageFormarControl;
        GENICAM_UserSetControl *userSetControl;

        int isError = 0;                          // エラー判定、APIでは無理やり？キャストすることで無効な値をマイナス値にしている 
        unsigned char *imageBuffer;               // 取得した画像のリスト、CMOSセンサーの値が入ってくる
        uint32_t cameraCount = 0;
        bool isConnected = false;
        int imageHeight;
        int imageWidth;
        Mat cameraMat;

        void setGamma(double gamma) {
            if(NULL == *camera) {
                return;
            }
            GENICAM_DoubleNode doubleNode = analogControl->gamma(analogControl);
            int32_t isSetFailed = doubleNode.setValue(&doubleNode, gamma);
            if (0 != isSetFailed) {
                // エラーハンドリングとか
            } 
            doubleNode.release(&doubleNode);
        }

        void setGain(double gain) {
            if(NULL == *camera || gain < 0) {
                return;
            }
            GENICAM_EnumNode enumNode = analogControl->gainAuto(analogControl);
            enumNode.setValueBySymbol(&enumNode, GAINAUTO_OFF_STR);
            enumNode.release(&enumNode);
            GENICAM_DoubleNode doubleNode = analogControl->gainRaw(analogControl);
            int32_t isSetFailed = doubleNode.setValue(&doubleNode, gain);
            if (0 != isSetFailed) {
                // エラーハンドリングとか
            } 
            doubleNode.release(&doubleNode);
        }

        void setShutterTime(double shutterTime) {
            if(NULL == camera || shutterTime < 0) {
                return;
            }

            GENICAM_EnumNode enumNode = acquisitionControl->exposureAuto(acquisitionControl);
            enumNode.setValueBySymbol(&enumNode, EXPOSUREAUTO_OFF_STR);
            enumNode.release(&enumNode);

            GENICAM_DoubleNode doubleNode = acquisitionControl->exposureTime(acquisitionControl);
            int32_t isSetFailed = doubleNode.setValue(&doubleNode, shutterTime);
            if (0 != isSetFailed) {
                // エラーハンドリングとか
            }
            doubleNode.release(&doubleNode);
        }

        void setBrightness(int brightness) {
            if(NULL == *camera || brightness < 0) {
                return;
            }
            GENICAM_IntNode intNode = ispControl->brightness(ispControl);
            int32_t isSetFailed = intNode.setValue(&intNode, (int64_t)brightness);
            if (0 != isSetFailed) {
                // エラーハンドリングとか
            }
            intNode.release(&intNode); 
        }

        void setContrast(int contrast) {
            if(NULL == *camera || contrast < 0) {
                return;
            }
            GENICAM_IntNode intNode = ispControl->contrast(ispControl);
            int32_t isSetFailed = intNode.setValue(&intNode, (int64_t)contrast);
            if (0 != isSetFailed) {
                // エラーハンドリングとか
            }
            intNode.release(&intNode); 
        }
        
        void connectCamera() {
            isError = GENICAM_getSystemInstance(&system);
            if (isError != 0){
                return;
            }

            isError = system->discovery(system, &cameraList, &cameraCount, typeAll);
            if (isError != 0 || cameraCount < 0) {
                return;
            }
            camera = &cameraList[0];
            isError = camera->connect(camera, accessPermissionControl);

            if (isError != 0) {
                return;
            }
            isConnected = true;
        }

        void createControls() {
            if (isError != 0 || !isConnected) {
                return;
            }

            GENICAM_AcquisitionControlInfo acquisitionControlInfo = { 0 };
            acquisitionControlInfo.pCamera = camera;
            GENICAM_createAcquisitionControl(&acquisitionControlInfo, &acquisitionControl);

            GENICAM_AnalogControlInfo analogControlInfo = { 0 };
            analogControlInfo.pCamera = camera;
            GENICAM_createAnalogControl(&analogControlInfo, &analogControl);

            GENICAM_ISPControlInfo ispControlInfo = { 0 };
            ispControlInfo.pCamera = camera;
            GENICAM_createISPControl(&ispControlInfo, &ispControl);

            GENICAM_ImageFormatControlInfo imageFormatControlInfo = { 0 };
            imageFormatControlInfo.pCamera = camera;
            GENICAM_createImageFormatControl(&imageFormatControlInfo, &imageFormarControl);

            GENICAM_UserSetControlInfo userSetControlInfo = { 0 };
            userSetControlInfo.pCamera = camera;
            GENICAM_createUserSetControl(&userSetControlInfo, &userSetControl);
        }

        void getWidthFromSystem() {
            if (camera == NULL || !isConnected) {
                return;
            }
            int64_t width = 0;
            GENICAM_IntNode intNode = imageFormarControl->width(imageFormarControl);
            intNode.getValue(&intNode, &width);
            intNode.release(&intNode);
            imageWidth = (int)width;
        }

        void getHeightFromSystem() {
            if (camera == NULL || !isConnected) {
                return;
            }
            int64_t height = 0;
            GENICAM_IntNode intNode = imageFormarControl->width(imageFormarControl);
            intNode.getValue(&intNode, &height);
            intNode.release(&intNode);
            imageHeight = (int)height;
        }

        void startCapture() {
            if (!isConnected || isError != 0) {
                return;
            }
            GENICAM_StreamSourceInfo streamInfo;

            streamInfo.pCamera = camera;
            streamInfo.channelId = 0;
            GENICAM_createStreamSource(&streamInfo, &streamSource);
            streamSource->startGrabbing(streamSource, 0, grabStrartegySequential);
            
            GENICAM_Frame *frame;
            int count = 0;

            if (streamSource->isGrabbing(streamSource) == 0) {
                return;
            }
            
            while (isThreadRunning())
            {
                timer.waitNext();
                if (lock()) {
                    isError = streamSource->getFrame(streamSource, &frame, 0);
                    if (isError != 0) {
                        frame->release(frame);
                        continue;
                    }

                    isError = frame->valid(frame);

                    if (isError != 0) {
                        frame->release(frame);
                        continue;
                    }

                    uint32_t imageBufferSize = frame->getImageSize(frame);
                    imageBuffer = new unsigned char[imageBufferSize];
                    memcpy(imageBuffer, frame->getImage(frame), imageBufferSize);
                    if (count > 1) {
                        Mat bayerImage = Mat(imageHeight, imageWidth, CV_8U, imageBuffer);
                        Mat convertImage;
                        cvtColor(bayerImage, convertImage, COLOR_BaterRG2GRAY);
                        bayerImage.copyTo(cameraMat);
                        toOf(cameraMat, cameraImage);
                    }
                    count++;
                    frame->release(frame);
                    unlock();
                }
            }
        }

        void stopCapture() {
            if (streamSource->isGrabbing(streamSource) != 0) {
                return;
            }
            streamSource->stopGrabbing(streamSource);
        }

        void disConnectCamera() {
            streamSource->release(streamSource);
            ispControl->release(ispControl);
            analogControl->release(analogControl);
            imageFormarControl->release(imageFormarControl);
            acquisitionControl->release(acquisitionControl);
            userSetControl->release(userSetControl);
            camera->disConnect(camera);
            camera->release(camera);
            isConnected = false;
        }
};
