// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_INPUTMOTION_HPP_
#define HERVE_INPUTMOTION_HPP_

#include <chrono>

namespace Herve {

/// \brief input event for controlling a value (for example, key press/release for controlling forward camera translation)
///
class InputMotion1d {
protected:
	float _value;
	float _speed;
	bool _isMotion;
	std::chrono::time_point<std::chrono::system_clock> _t0;

public:
	/// \brief use this constructor
	///
	InputMotion1d(float speed);

	/// \brief press event: start motion
	///
	void press();

	/// \brief release event: stop motion
	///
	void release();

	/// \brief get motion value
	///
	virtual float getValue() = 0;

protected:
	void update();
};

/// \brief input event for controlling a value (for example, key press/release for controlling forward camera translation)
///
/// the value is reset when getValue is called
class InputMotion1dRelative : public InputMotion1d {
public:
	/// \brief use this constructor
	///
	InputMotion1dRelative(float speed);

	/// \brief get motion value (and reset)
	///
	float getValue();
};

/// \brief input event for controlling a value (for example, key press/release for controlling forward camera translation)
///
/// calling getValue do not reset the value
class InputMotion1dAbsolute : public InputMotion1d {
public:
	/// \brief use this constructor
	///
	InputMotion1dAbsolute(float speed);

	/// \brief get motion value
	///
	float getValue();
};

/// \brief input event for controlling 2 values (for example, mouse motion)
///
class InputMotion2d {
	float _xValue;
	float _yValue;
	float _xSpeed;
	float _ySpeed;
	float _x0;
	float _y0;
	bool _isMotion;

public:
	/// \brief use this constructor
	///
	InputMotion2d(float xInitialValue, float yInitialValue, float xSpeed, float ySpeed);

	/// \brief press event
	///
	void press(int x, int y);

	/// \brief motion event
	///
	void motion(int x, int y);

	/// \brief release event
	///
	void release(int x, int y);

	/// \brief reset value and input position
	///
	void reset();

	/// \brief get ABSOLUTE value for x (since call to press)
	///
	float getXValue() const;

	/// \brief get ABSOLUTE value for y (since call to press)
	///
	float getYValue() const;
};

} // namespace Herve

#endif
