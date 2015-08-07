// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/InputMotion.hpp>

using namespace Herve;

//////////////////////////////////////////////////////////////////////
// InputMotion1d
//////////////////////////////////////////////////////////////////////

InputMotion1d::InputMotion1d(float speed) :
_value(0), _speed(speed), _isMotion(false) {
}

void InputMotion1d::press() {
	if (not _isMotion) {
		_t0 = std::chrono::system_clock::now();
		_isMotion = true;
	}
}

void InputMotion1d::release() {
	if (_isMotion) {
		update();
		_isMotion = false;
	}
}

void InputMotion1d::update() {
	if (_isMotion) {
		std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
		float deltaT = 1e-3 * std::chrono::duration_cast < std::chrono::milliseconds > (t1 - _t0).count();
		_value += deltaT*_speed;
		_t0 = t1;
	}
}


//////////////////////////////////////////////////////////////////////
// InputMotion1dRelative
//////////////////////////////////////////////////////////////////////

InputMotion1dRelative::InputMotion1dRelative(float speed) :
		InputMotion1d(speed) {
}

float InputMotion1dRelative::getValue() {
	update();
	float value = _value;
	_value = 0;
	return value;
}


//////////////////////////////////////////////////////////////////////
// InputMotion1dAbsolute
//////////////////////////////////////////////////////////////////////

InputMotion1dAbsolute::InputMotion1dAbsolute(float speed) :
		InputMotion1d(speed) {
}

float InputMotion1dAbsolute::getValue() {
	update();
	return _value;
}



//////////////////////////////////////////////////////////////////////
// InputMotion2d
//////////////////////////////////////////////////////////////////////

InputMotion2d::InputMotion2d(float xInitialValue, float yInitialValue, float xSpeed, float ySpeed) :
_xValue(xInitialValue), _yValue(yInitialValue), _xSpeed(xSpeed), _ySpeed(ySpeed), _isMotion(false) {
}

void InputMotion2d::press(int x, int y) {
	_x0 = x;
	_y0 = y;
	_isMotion = true;
}

void InputMotion2d::motion(int x, int y) {
	if (_isMotion) {
		_xValue += (x - _x0)*_xSpeed;
		_yValue += (y - _y0)*_ySpeed;
		_x0 = x;
		_y0 = y;
	}
}

void InputMotion2d::release(int x, int y) {
	motion(x, y);
	_isMotion = false;
}

void InputMotion2d::reset()
{
	_xValue=0;
	_yValue=0;
	_x0=0;
	_y0=0;
}

float InputMotion2d::getXValue() const {
	return _xValue;
}

float InputMotion2d::getYValue() const {
	return _yValue;
}
