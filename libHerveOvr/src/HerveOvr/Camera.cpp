// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/glm.hpp>
#include <HerveOvr/Camera.hpp>

using namespace HerveOvr;

void CameraOvr::updateView(float tx, float ty, float tz, float rx, float ry, float rz) {
	_rx = rx;
	_ry = ry;
	_rz = rz;

	float fullRy = _hmdRy + _ry;
	_tx += tx*cosf(fullRy) + tz*sinf(fullRy);
	_ty += ty;
	_tz += tz*cosf(fullRy) - tx*sinf(fullRy);
}

glm::mat4 CameraOvr::getOrientation(OVR::Quatf orientationQuat, const OVR::Util::Render::StereoEyeParams& eyeParams) {

	orientationQuat.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&_hmdRy, &_hmdRx, &_hmdRz);

	OVR::Matrix4f orientation = OVR::Matrix4f::RotationY(_hmdRy+_ry)
		* OVR::Matrix4f::RotationX(_hmdRx+_rx)
		* OVR::Matrix4f::RotationZ(_hmdRz+_rz);

	OVR::Matrix4f view = orientation.Inverted() * eyeParams.ViewAdjust;

	return ovrToGlmMat4(view);
}

glm::mat4 CameraOvr::ovrToGlmMat4(const OVR::Matrix4f& mat) {
	return glm::mat4(
			mat.M[0][0], mat.M[1][0], mat.M[2][0], mat.M[3][0],
			mat.M[0][1], mat.M[1][1], mat.M[2][1], mat.M[3][1],
			mat.M[0][2], mat.M[1][2], mat.M[2][2], mat.M[3][2],
			mat.M[0][3], mat.M[1][3], mat.M[2][3], mat.M[3][3]);
}


