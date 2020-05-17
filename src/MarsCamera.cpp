#pragma once

#include "./ofxMarsCamera.h"
#include "ofxCv.h"

using namespace ofxCv;
using namespace cv;

void MarsCamera::setThreadExecuteTime(int time)
{
    timer.setPeriodEvent(time) //単位はns
}

void MarsCamera::connect()
{
    connectCamera();
    getHeightFromSystem();
    getWidthFromSystem();
    createControls();
}

void MarsCamera::setCameraValues(
    double gamma,
    double gain,
    double shutterTime,
    int brightness,
    int contrast)
{
    setGamma(gamma);
    setGain(gain);
    setShutterTime(shutterTime);
    setBrightness(brightness);
    setContrast(contrast);
}

void MarsCamera::openStream()
{
    startCapture();
}

void MarsCamera::stopStream()
{
    stopCapture();
}

void MarsCamera::disConnect()
{
    disConnectCamera();
}

void MarsCamera::setGamma(double gamma)
{
    if (NULL == *camera)
    {
        return;
    }
    GENICAM_DoubleNode doubleNode = analogControl->gamma(analogControl);
    int32_t isSetFailed = doubleNode.setValue(&doubleNode, gamma);
    if (0 != isSetFailed)
    {
        // エラーハンドリングとか
    }
    doubleNode.release(&doubleNode);
}

void MarsCamera::setGain(double gain)
{
    if (NULL == *camera || gain < 0)
    {
        return;
    }
    GENICAM_EnumNode enumNode = analogControl->gainAuto(analogControl);
    enumNode.setValueBySymbol(&enumNode, GAINAUTO_OFF_STR);
    enumNode.release(&enumNode);
    GENICAM_DoubleNode doubleNode = analogControl->gainRaw(analogControl);
    int32_t isSetFailed = doubleNode.setValue(&doubleNode, gain);
    if (0 != isSetFailed)
    {
        // エラーハンドリングとか
    }
    doubleNode.release(&doubleNode);
}

void MarsCamera::setShutterTime(double shutterTime)
{
    if (NULL == camera || shutterTime < 0)
    {
        return;
    }

    GENICAM_EnumNode enumNode = acquisitionControl->exposureAuto(acquisitionControl);
    enumNode.setValueBySymbol(&enumNode, EXPOSUREAUTO_OFF_STR);
    enumNode.release(&enumNode);

    GENICAM_DoubleNode doubleNode = acquisitionControl->exposureTime(acquisitionControl);
    int32_t isSetFailed = doubleNode.setValue(&doubleNode, shutterTime);
    if (0 != isSetFailed)
    {
        // エラーハンドリングとか
    }
    doubleNode.release(&doubleNode);
}

void MarsCamera::setBrightness(int brightness)
{
    if (NULL == *camera || brightness < 0)
    {
        return;
    }
    GENICAM_IntNode intNode = ispControl->brightness(ispControl);
    int32_t isSetFailed = intNode.setValue(&intNode, (int64_t)brightness);
    if (0 != isSetFailed)
    {
        // エラーハンドリングとか
    }
    intNode.release(&intNode);
}

void MarsCamera::setContrast(int contrast)
{
    if (NULL == *camera || contrast < 0)
    {
        return;
    }
    GENICAM_IntNode intNode = ispControl->contrast(ispControl);
    int32_t isSetFailed = intNode.setValue(&intNode, (int64_t)contrast);
    if (0 != isSetFailed)
    {
        // エラーハンドリングとか
    }
    intNode.release(&intNode);
}

void MarsCamera::connectCamera()
{
    isError = GENICAM_getSystemInstance(&system);
    if (isError != 0)
    {
        return;
    }

    isError = system->discovery(system, &cameraList, &cameraCount, typeAll);
    if (isError != 0 || cameraCount < 0)
    {
        return;
    }
    camera = &cameraList[0];
    isError = camera->connect(camera, accessPermissionControl);

    if (isError != 0)
    {
        return;
    }
    isConnected = true;
}

void MarsCamera::createControls()
{
    if (isError != 0 || !isConnected)
    {
        return;
    }

    GENICAM_AcquisitionControlInfo acquisitionControlInfo = {0};
    acquisitionControlInfo.pCamera = camera;
    GENICAM_createAcquisitionControl(&acquisitionControlInfo, &acquisitionControl);

    GENICAM_AnalogControlInfo analogControlInfo = {0};
    analogControlInfo.pCamera = camera;
    GENICAM_createAnalogControl(&analogControlInfo, &analogControl);

    GENICAM_ISPControlInfo ispControlInfo = {0};
    ispControlInfo.pCamera = camera;
    GENICAM_createISPControl(&ispControlInfo, &ispControl);

    GENICAM_ImageFormatControlInfo imageFormatControlInfo = {0};
    imageFormatControlInfo.pCamera = camera;
    GENICAM_createImageFormatControl(&imageFormatControlInfo, &imageFormarControl);

    GENICAM_UserSetControlInfo userSetControlInfo = {0};
    userSetControlInfo.pCamera = camera;
    GENICAM_createUserSetControl(&userSetControlInfo, &userSetControl);
}

void MarsCamera::getWidthFromSystem()
{
    if (camera == NULL || !isConnected)
    {
        return;
    }
    int64_t width = 0;
    GENICAM_IntNode intNode = imageFormarControl->width(imageFormarControl);
    intNode.getValue(&intNode, &width);
    intNode.release(&intNode);
    imageWidth = (int)width;
}

void MarsCamera::getHeightFromSystem()
{
    if (camera == NULL || !isConnected)
    {
        return;
    }
    int64_t height = 0;
    GENICAM_IntNode intNode = imageFormarControl->width(imageFormarControl);
    intNode.getValue(&intNode, &height);
    intNode.release(&intNode);
    imageHeight = (int)height;
}

void MarsCamera::startCapture()
{
    if (!isConnected || isError != 0)
    {
        return;
    }
    GENICAM_StreamSourceInfo streamInfo;

    streamInfo.pCamera = camera;
    streamInfo.channelId = 0;
    GENICAM_createStreamSource(&streamInfo, &streamSource);
    streamSource->startGrabbing(streamSource, 0, grabStrartegySequential);

    GENICAM_Frame *frame;
    int count = 0;

    if (streamSource->isGrabbing(streamSource) == 0)
    {
        return;
    }

    while (isThreadRunning())
    {
        timer.waitNext();
        if (lock())
        {
            isError = streamSource->getFrame(streamSource, &frame, 0);
            if (isError != 0)
            {
                frame->release(frame);
                continue;
            }

            isError = frame->valid(frame);

            if (isError != 0)
            {
                frame->release(frame);
                continue;
            }

            uint32_t imageBufferSize = frame->getImageSize(frame);
            imageBuffer = new unsigned char[imageBufferSize];
            memcpy(imageBuffer, frame->getImage(frame), imageBufferSize);
            if (count > 1)
            {
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

void MarsCamera::stopCapture()
{
    if (streamSource->isGrabbing(streamSource) != 0)
    {
        return;
    }
    streamSource->stopGrabbing(streamSource);
}

void MarsCamera::disConnectCamera()
{
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
