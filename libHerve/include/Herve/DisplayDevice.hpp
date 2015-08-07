// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_DISPLAYDEVICE_HPP_
#define HERVE_DISPLAYDEVICE_HPP_

#include <Herve/Camera.hpp>
#include <Herve/FrameBuffer.hpp>
#include <Herve/Scene.hpp>
#include <Herve/Shader.hpp>

namespace Herve {

/// \brief DisplayDevice (abstract class): to be derived for monoscopic display, stereo, HMD...
///
class DisplayDevice {

protected:
	int _width, _height;
	int _maxWidth, _maxHeight;

public:
	virtual ~DisplayDevice(){};

	/// \brief current display width
	///
	int getWidth() const;

	/// \brief current display height
	///
	int getHeight() const;

	/// \brief max display width
	///
	int getMaxWidth() const;

	/// \brief max display width
	///
	int getMaxHeight() const;

	/// \brief update current display size
	///
	void resizeWindow(int width, int height);

	/// \brief init device (usefull for HMD)
	///
	virtual bool initDevice() = 0;

	/// \brief init display (OpenGL)
	///
	virtual void initDisplay(int maxWidth, int maxHeight);

	/// \brief update walkthrough camera (translations in m, rotations in rad)
	///
	virtual void updateView(float tx, float ty, float tz, float rx, float ry, float rz) = 0;

	/// \brief reset camera
	///
	virtual void reset(float tx, float ty, float tz, float rx, float ry, float rz) = 0;

	/// \brief update display
	///
	virtual void render(Scene * ptrScene) = 0;

	/// \brief get camera orientation
	///
	virtual glm::mat4 getOrientation() = 0;

	/// \brief get internal euler angles (for HMD)
	///
	virtual void getInternalEulerAnglesYXZ(float & yaw, float & pitch, float & roll);

	/// \brief get camera translation
	///
	virtual glm::mat4 getTranslation() = 0;
};

/// \brief monoscopic display
///
class DisplayDeviceMono : public DisplayDevice {

	Camera _camera;

public:
	virtual ~DisplayDeviceMono(){};

	/// \brief just set a default size
	///
	bool initDevice();

	/// \brief update camera
	///
	void updateView(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief reset camera
	///
	void reset(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief update display
	///
	void render(Scene * ptrScene);

	/// \brief get camera orientation
	///
	glm::mat4 getOrientation();

	/// \brief get camera translation
	///
	glm::mat4 getTranslation();
};


/// \brief stereoscopic display (abstract class)
///
class DisplayDeviceStereo : public DisplayDevice {

protected:
	// frame buffers for rendering scene to textures
	FrameBuffer _leftFbo;
	FrameBuffer _rightFbo;
	int _fboWidth;
	int _fboHeight;

	// data for rendering textures to screen
	Mesh _quadMesh;
	Shader _quadShader;

	float _distortionScale;
	float _distortionK[4];

public:
	virtual ~DisplayDeviceStereo(){};

	/// \brief constructor (set FBO size)
	///
	DisplayDeviceStereo(int fboWidth, int fboHeight);

	/// \brief set max size, init FBO...
	///
	void initDisplay(int maxWidth, int maxHeight);

	/// \brief update display
	///
	void render(Scene * ptrScene);

	/// \brief render scene in the two FBO (left eye, right eye)
	///
	virtual void renderFbo(Scene * ptrScene, int & viewportHeight, int & viewportWidth2,
			float & lensCenterXLeft, float & lensCenterXRight) = 0;
};

/// \brief fake stereoscopic display for testing purpose (render two views with default parameters)
///
class DisplayDeviceStereoFake : public DisplayDeviceStereo {

	Camera _camera;

public:
	~DisplayDeviceStereoFake(){};

	/// \brief constructor (set FBO size)
	///
	DisplayDeviceStereoFake(int fboWidth, int fboHeight);

	/// \brief just set default display size...
	///
	bool initDevice();

	/// \brief update walkthrough camera (translations in m, rotations in rad)
	///
	void updateView(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief reset camera
	///
	void reset(float tx, float ty, float tz, float rx, float ry, float rz);

	/// \brief render scene in the two FBO (left eye, right eye)
	///
	void renderFbo(Scene * ptrScene, int & viewportHeight, int & viewportWidth2, float & lensCenterXLeft, float & lensCenterXRight);

	/// \brief camera orientation
	///
	glm::mat4 getOrientation();

	/// \brief camera translation
	///
	glm::mat4 getTranslation();

private:
	void renderEyeToFbo(Scene * ptrScene, float tx, FrameBuffer& fbo, int w, int h);
};

} // namespace Herve

#endif
