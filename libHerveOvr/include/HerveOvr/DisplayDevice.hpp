// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVEOVR_DISPLAYDEVICE_HPP_
#define HERVEOVR_DISPLAYDEVICE_HPP_

#include <Herve/Herve.hpp>
#include <HerveOvr/Camera.hpp>
#include <OVR.h>

namespace HerveOvr {

/// \brief DisplayDevice for oculus rift HMD
///
class DisplayDeviceStereoOvr : public Herve::DisplayDeviceStereo {

	// OVR
	OVR::Ptr<OVR::DeviceManager> _ptrManager;
	OVR::Ptr<OVR::HMDDevice> _ptrHmd;
	OVR::Ptr<OVR::SensorDevice> _ptrSensor;
	OVR::SensorFusion _sensorFusion;
	OVR::Util::Render::StereoConfig _stereoConfig;
	OVR::Util::Render::DistortionConfig _distortionConfig;
	OVR::HMDInfo _hmdInfo;

	int _hmdWidth, _hmdHeight;

	// other data
	CameraOvr _cameraOvr;

public:
	~DisplayDeviceStereoOvr(){};

	/// \brief use this constructor
	///
	DisplayDeviceStereoOvr(int fboWidth, int fboHeight);

	/// \brief init HMD
	///
	bool initDevice();

	/// \brief update walkthrough camera
	///
	void updateView(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief reset camera
	///
	void reset(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief render scene in the two fbo (left eye and right eye)
	///
	void renderFbo(Herve::Scene * ptrScene, int & viewportHeight, int & viewportWidth2, float & lensCenterXLeft, float & lensCenterXRight);

	/// \brief get camera orientation (center view)
	///
	glm::mat4 getOrientation();

	/// \brief get internal euler angles (for HMD)
	///
	void getInternalEulerAnglesYXZ(float & yaw, float & pitch, float & roll);

	/// \brief get camera translation
	///
	glm::mat4 getTranslation();

private:
	void renderEyeToFbo(Herve::Scene * ptrScene, OVR::Util::Render::StereoEye eye, Herve::FrameBuffer & fbo);
};

} // namespace HerveOvr

#endif
