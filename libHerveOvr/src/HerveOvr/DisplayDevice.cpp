// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Herve.hpp>
#include <HerveOvr/DisplayDevice.hpp>

using namespace HerveOvr;

//////////////////////////////////////////////////////////////////////
// DisplayDeviceStereoOvr
//////////////////////////////////////////////////////////////////////

bool DisplayDeviceStereoOvr::initDevice() {
	// default size
	_hmdWidth = 800;
	_hmdHeight = 600;

	// OVR system
	OVR::System::Init();
	_ptrManager = *OVR::DeviceManager::Create();
	if (not _ptrManager) {
		Herve::utilInfo("No OVR manager found", "", __FILE__, __LINE__);
		return false;
	}
	Herve::utilInfo("OVR manager found", "", __FILE__, __LINE__);

	// OVR HMD
	_ptrHmd = *_ptrManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
	if (not _ptrHmd) {
		Herve::utilInfo("No OVR HMD found", "", __FILE__, __LINE__);
		return false;
	}
	_ptrHmd->GetDeviceInfo(&_hmdInfo);
	Herve::utilInfo("OVR HMD found : ", _hmdInfo.DisplayDeviceName, __FILE__, __LINE__);

	_width = _maxWidth = _hmdWidth = _hmdInfo.HResolution;
	_height = _maxHeight = _hmdHeight = _hmdInfo.VResolution;
	
	// OVR sensor
	_ptrSensor = *_ptrHmd->GetSensor();
	if (not _ptrSensor) {
		Herve::utilInfo("No OVR sensor found", "", __FILE__, __LINE__);
		return false;
	}
	Herve::utilInfo("OVR sensor found", "", __FILE__, __LINE__);
	_sensorFusion.AttachToSensor(_ptrSensor);

	// OVR stereo rendering
	_stereoConfig.SetFullViewport(OVR::Util::Render::Viewport(0, 0, _hmdWidth, _hmdHeight));
	_stereoConfig.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
	_stereoConfig.SetHMDInfo(_hmdInfo);
	_stereoConfig.SetDistortionFitPointVP(-1.0f, 0.0f);
	_distortionConfig = _stereoConfig.GetDistortionConfig();

	_distortionScale = 1.f /_distortionConfig.Scale;

	_distortionK[0] = _distortionConfig.K[0];
	_distortionK[1] = _distortionConfig.K[1];
	_distortionK[2] = _distortionConfig.K[2];
	_distortionK[3] = _distortionConfig.K[3];

	return true;
}


void DisplayDeviceStereoOvr::renderFbo(Herve::Scene * ptrScene, int & viewportHeight, int & viewportWidth2,
		float & lensCenterXLeft, float & lensCenterXRight) {

	lensCenterXLeft = 0.5f*(1+_distortionConfig.XCenterOffset);
	lensCenterXRight = 0.5f*(1-_distortionConfig.XCenterOffset);

	viewportHeight = std::min(_height, _hmdHeight);
	viewportWidth2 = 0.5 * std::min(_width, _hmdWidth);

	// render each eye to its texture
	renderEyeToFbo(ptrScene, OVR::Util::Render::StereoEye_Left, _leftFbo);
	renderEyeToFbo(ptrScene, OVR::Util::Render::StereoEye_Right, _rightFbo);
}

DisplayDeviceStereoOvr::DisplayDeviceStereoOvr(int fboWidth, int fboHeight) :
	DisplayDeviceStereo(fboWidth, fboHeight) {
}


// translations in m, rotations in rad
void DisplayDeviceStereoOvr::updateView(float tx, float ty, float tz, float rx, float ry, float rz) {
	_cameraOvr.updateView(tx, ty, tz, rx, ry, rz);
}

void DisplayDeviceStereoOvr::reset(float tx, float ty, float tz, float rx, float ry, float rz) {
	_cameraOvr.reset(tx, ty, tz, rx, ry, rz);
}

void DisplayDeviceStereoOvr::renderEyeToFbo(Herve::Scene * ptrScene, OVR::Util::Render::StereoEye eye, Herve::FrameBuffer& fbo) {

	// compute projection-view matrix from sensor inputs
	OVR::Util::Render::StereoEyeParams eyeParams = _stereoConfig.GetEyeRenderParams(eye);
	OVR::Quatf hmdOrientationQuat = _sensorFusion.GetOrientation();
	glm::mat4 V = _cameraOvr.getOrientation(hmdOrientationQuat, eyeParams) * _cameraOvr.getTranslation();
	glm::mat4 P = CameraOvr::ovrToGlmMat4(eyeParams.Projection);

	// render scene to texture
	fbo.bindFrameBuffer();
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _fboWidth, _fboHeight);
	ptrScene->render(V, P);
	fbo.unbindFrameBuffer();
}

void DisplayDeviceStereoOvr::getInternalEulerAnglesYXZ(float & yaw, float & pitch, float & roll) {

	OVR::Quatf hmdOrientationQuat = _sensorFusion.GetOrientation();
	hmdOrientationQuat.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);
}

glm::mat4 DisplayDeviceStereoOvr::getOrientation() {

	OVR::Util::Render::StereoEyeParams eyeParams = _stereoConfig.GetEyeRenderParams(OVR::Util::Render::StereoEye_Center);
	OVR::Quatf hmdOrientationQuat = _sensorFusion.GetOrientation();
	return _cameraOvr.getOrientation(hmdOrientationQuat, eyeParams);
}

glm::mat4 DisplayDeviceStereoOvr::getTranslation() {
	return _cameraOvr.getTranslation();
}
