// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Camera.hpp>

using namespace Herve;

Camera::Camera() {
	_tx = _ty = _tz = 0;
	_rx = _ry = _rz = 0;
}

glm::mat4 Camera::getTranslation() const {
	return glm::translate(glm::mat4(1.f), glm::vec3(-_tx, -_ty, -_tz));
}

void Camera::updateView(float tx, float ty, float tz, float rx, float ry, float rz) {
	_rx = rx;
	_ry = ry;
	_rz = rz;

	float fullRy =  _ry;
	_tx += tx*cosf(fullRy) + tz*sinf(fullRy);
	_ty += ty;
	_tz += tz*cosf(fullRy) - tx*sinf(fullRy);
}

void Camera::reset(float tx, float ty, float tz, float rx, float ry, float rz)
{
	_tx = tx;
	_ty = ty;
	_tz = tz;
	_rx = rx;
	_ry = ry;
	_rz = rz;
}

glm::vec3 Camera::getTranslationVector() const {
	return glm::vec3(-_tx, -_ty, -_tz);
}
	
glm::mat4 Camera::getOrientation() {

	// FIXME glm/debian rotations en degrees
	/*
	glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::degrees(-_rx), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::degrees(-_ry), glm::vec3(0.f, 1.f, 0.f));
	*/

	glm::mat4 Rx = glm::rotate(glm::mat4(1.f), -_rx, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.f), -_ry, glm::vec3(0.f, 1.f, 0.f));

	return Rx * Ry;
}

