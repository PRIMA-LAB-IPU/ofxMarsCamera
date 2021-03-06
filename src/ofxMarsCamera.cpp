#pragma once

#include "ofxMarsCamera.h"

//------------------------------------------------------------

void MarsCamera::connect()
{
	if (!isConnected) {
		connectCamera();
		createControls();
		getHeightFromSystem();
		getWidthFromSystem();
	}
}

bool MarsCamera::update(GENICAM_Frame* frame, cv::Mat& frame_cv) {

	if (!bStreaming)
	{
		ofLogError() << "Error : bStreaming";
		return false;
	}

	isError = streamSource->getFrame(streamSource, &frame, 0);
	if (isError != 0)
	{
		//ofLogError() << "Error : getFrame";
		if (frame != NULL)
			frame->release(frame);
		return false;
	}

	isError = frame->valid(frame);

	if (isError != 0)
	{
		ofLogError() << "Error : valid";
		if (frame != NULL)
			frame->release(frame);
		return false;
	}

	//Get Frame info
	frame_param.dataSize = frame->getImageSize(frame);
	frame_param.height = frame->getImageHeight(frame);
	frame_param.width = frame->getImageWidth(frame);
	frame_param.paddingX = frame->getImagePaddingX(frame);
	frame_param.paddingY = frame->getImagePaddingY(frame);
	frame_param.pixelForamt = frame->getImagePixelFormat(frame);

	//Get YUYV Frame
	memcpy(imageRawBuffer, frame->getImage(frame), frame_param.dataSize);

    // YUYV ? to RGB
	int nDataSize = 0;
	IMGCNV_ConvertToBGR24(imageRawBuffer, &frame_param, imageBayerBuffer_filted, &nDataSize);
	memcpy(frame_cv.data, imageBayerBuffer_filted, frame_param.dataSize * 3);
	cv::cvtColor(frame_cv, frame_cv, cv::COLOR_BGR2RGB);

	frameID++;
	frame->release(frame);

	fps.newFrame();

	return true;
}

void MarsCamera::openStream()
{
	if (isConnected && !bStreaming) {
		frameID = 0;

		//Allocate
		imageRawBuffer = new unsigned char[IMAGE_SIZE];
		imageBayerBuffer_filted = new unsigned char[IMAGE_SIZE * 3];
		frameMat = cv::Mat::zeros(cv::Size(imageWidth,imageHeight), CV_8UC3);

		startCapture();
		startThread();
	}
	else {
		ofLogError() << "Error : openStream";
	}
}

void MarsCamera::stopStream()
{
	if (isConnected && bStreaming) {
		ofLog() << "Stop thread";
		stopThread();
		ofLog() << "Stop capture";
		stopCapture();

		bStreaming = false;
	}
	else {
		ofLogError() << "Error : stopStream";
	}
}

void MarsCamera::disConnect()
{
	if (isConnected) {
		ofLog() << "Disconnect camera";
		disConnectCamera();
	}
	else {
		ofLogError() << "Error : disConnect";
	}
}

//------------------------------------------------------------

float MarsCamera::getFPS() {
	return fps.getFps();
}

float MarsCamera::getFrameID() {
	return frameID;
}

//------------------------------------------------------------

void MarsCamera::setGamma(double gamma)
{
	gamma = ofClamp(gamma, 0, 4);

	if (NULL == camera)
	{
		ofLogError() << "Error : setGamma";
		return;
	}
	GENICAM_DoubleNode doubleNode = analogControl->gamma(analogControl);
	int32_t isSetFailed = doubleNode.setValue(&doubleNode, gamma);
	if (0 != isSetFailed)
	{
		ofLogError() << "Error : setGamma";
	}
	doubleNode.release(&doubleNode);
}

void MarsCamera::setShutterTime(double shutterTime)
{
	shutterTime = ofClamp(shutterTime, 16.00, 1000000.00);

	if (NULL == camera || shutterTime < 0)
	{
		ofLogError() << "Error : setShutterTime";
		return;
	}

	GENICAM_EnumNode enumNode = acquisitionControl->exposureAuto(acquisitionControl);
	enumNode.setValueBySymbol(&enumNode, EXPOSUREAUTO_OFF_STR);
	enumNode.release(&enumNode);

	GENICAM_DoubleNode doubleNode = acquisitionControl->exposureTime(acquisitionControl);
	int32_t isSetFailed = doubleNode.setValue(&doubleNode, shutterTime);
	if (0 != isSetFailed)
	{
		ofLogError() << "Error : setShutterTime";
	}
	doubleNode.release(&doubleNode);
}

void MarsCamera::setFPS(int fps) {
	fps = ofClamp(fps, 1.00, 816);
	setShutterTime(1000 / fps * 1000);
}

void MarsCamera::setBrightness(int brightness)
{
	brightness = ofClamp(brightness, 0.00, 100.00);

	if (NULL == camera || brightness < 0)
	{
		ofLogError() << "Error : setBrightness";
		return;
	}
	GENICAM_IntNode intNode = ispControl->brightness(ispControl);
	int32_t isSetFailed = intNode.setValue(&intNode, (int64_t)brightness);
	if (0 != isSetFailed)
	{
		ofLogError() << "Error : setShutterTime";
	}
	intNode.release(&intNode);
}

//------------------------------------------------------------

void MarsCamera::threadedFunction() {
	while (isThreadRunning())
	{
		if (update(frame, frameMat)) {
			frameBuffer.send(frameMat.clone());
		}
	}
}

void MarsCamera::connectCamera()
{
	uint32_t cameraCount;

	isError = GENICAM_getSystemInstance(&system);
	if (isError != 0)
	{
		ofLogError() << "Error : GENICAM_getSystemInstance";
		return;
	}

	isError = system->discovery(system, &cameraList, &cameraCount, typeAll);
	if (isError != 0 || cameraCount < 0)
	{
		ofLogError() << "Error : discovery";
		return;
	}
	camera = &cameraList[0];
	isError = camera->connect(camera, accessPermissionControl);

	if (isError != 0)
	{
		ofLogError() << "Error : connect";
		return;
	}

	isConnected = true;

	ofLog() << "Connect camera";
}

void MarsCamera::createControls()
{
	if (isError != 0 || !isConnected)
	{
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

	ofLog() << "Init camera options";
}

void MarsCamera::startCapture()
{
	if (!isConnected || isError != 0)
	{
		ofLogError() << "Error : isConnected";
		bStreaming = false;
		return;
	}

	GENICAM_StreamSourceInfo streamInfo;

	streamInfo.pCamera = camera;
	streamInfo.channelId = 0;
	GENICAM_createStreamSource(&streamInfo, &streamSource);
	uint32_t nRet = streamSource->startGrabbing(streamSource, 0, grabStrartegySequential);

	if (nRet != 0)
	{
		ofLogError() << "Error : isGrabbing";
		bStreaming = false;
		return;
	}

	bStreaming = true;

	ofLog() << "Start streaming";
}

void MarsCamera::stopCapture()
{
	if (isConnected && bStreaming) {
		if (streamSource->isGrabbing(streamSource) != 0)
		{
			ofLogError() << "Error : isGrabbing";
			return;
		}

		uint32_t nRet = streamSource->stopGrabbing(streamSource);
		if (nRet != 0)
		{
			ofLogError() << "Error : stopGrabbing";
			return;
		}
	}
}

void MarsCamera::disConnectCamera()
{
	if (isConnected) {
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
	GENICAM_IntNode intNode = imageFormarControl->height(imageFormarControl);
	intNode.getValue(&intNode, &height);
	intNode.release(&intNode);
	imageHeight = (int)height;
}