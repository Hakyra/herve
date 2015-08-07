// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_CAMERA_HPP_
#define HERVE_CAMERA_HPP_

#include <Herve/glm.hpp>

namespace Herve {

/// \brief walkthrough camera
//
class Camera {
protected:
	float _tx, _ty, _tz;
	float _rx, _ry, _rz;

public:
	/// \brief use this constructor
	///
	Camera();

	/// \brief update walkthrough camera
	///
	virtual void updateView(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief reset camera
	///
	void reset(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief translatation matrix
	///
	glm::mat4 getTranslation() const;

	/// \brief translatation vector
	///
	glm::vec3 getTranslationVector() const;

	/// \brief orientation
	///
	glm::mat4 getOrientation();
};

} // namespace Herve

#endif
