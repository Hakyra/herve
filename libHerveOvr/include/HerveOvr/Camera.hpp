// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVEOVR_CAMERA_HPP_
#define HERVEOVR_CAMERA_HPP_

#include <Herve/Camera.hpp>
#include <OVR.h>

namespace HerveOvr {

/// \brief Camera for oculus rift HMD
///
class CameraOvr : public Herve::Camera {

	float _hmdRx, _hmdRy, _hmdRz;

public:
	/// \brief update walkthrough camera
	///
	void updateView(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief get HMD orientation
	///
	glm::mat4 getOrientation(OVR::Quatf orientationQuat, const OVR::Util::Render::StereoEyeParams& eyeParams) ;

	/// \brief convert ovr matrix to glm matrix
	///
	static glm::mat4 ovrToGlmMat4(const OVR::Matrix4f & mat);
};

} // namespace HerveOvr

#endif
