// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/DisplayDevice.hpp>
#include <Herve/glm.hpp>
#include <Herve/Util.hpp>

using namespace Herve;

//////////////////////////////////////////////////////////////////////
// DisplayDevice
//////////////////////////////////////////////////////////////////////

void DisplayDevice::initDisplay(int maxWidth, int maxHeight) {
	// initialize OpenGL
	glEnable(GL_DEPTH_TEST);
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
		utilError("OpenGL error = ", (const char *)(gluErrorString(error)), __FILE__, __LINE__);

	// initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
		utilError("GLEW error = ", (const char *)glewGetErrorString(err), __FILE__, __LINE__);
	utilInfo("GLEW version = ", (const char *)glewGetString(GLEW_VERSION), __FILE__, __LINE__);

	_maxWidth = maxWidth;
	_maxHeight = maxHeight;
}

int DisplayDevice::getWidth() const {
	return _width;
}

int DisplayDevice::getHeight() const {
	return _height;
}

int DisplayDevice::getMaxWidth() const {
	return _maxWidth;
}

int DisplayDevice::getMaxHeight() const {
	return _maxHeight;
}

void DisplayDevice::resizeWindow(int width, int height) {
	_width = width;
	_height = std::max(2, height);
}


void DisplayDevice::getInternalEulerAnglesYXZ(float & yaw, float & pitch, float & roll) {
	yaw = pitch = roll = 0.f;
}

//////////////////////////////////////////////////////////////////////
// DisplayDeviceMono
//////////////////////////////////////////////////////////////////////

bool DisplayDeviceMono::initDevice() {
	// default size
	_width = _maxWidth = 800;
	_height = _maxHeight = 600;
	return true;
}

// translations in m, rotations in rad
void DisplayDeviceMono::updateView(float tx, float ty, float tz, float rx, float ry, float rz) {
	_camera.updateView(tx, ty, tz, rx, ry, rz);
}

void Herve::DisplayDeviceMono::reset(float tx, float ty, float tz, float rx, float ry, float rz)
{
	_camera.reset( tx,  ty,  tz,  rx,  ry,  rz);
}


void DisplayDeviceMono::render(Scene * ptrScene) {

	// projection-view matrix
	glm::mat4 V = _camera.getOrientation() * _camera.getTranslation();

	// FIXME : il y a une rotation sur z avec glm > 0.9.3 et fov < 50
	glm::mat4 P = glm::rotate(glm::mat4(1.f), float(M_PI), glm::vec3(0.f, 0.f, 1.f))
		* glm::perspective(80.f, _width/float(_height), 0.1f, 100.f);

	// prerender the scene (render framebuffers...)
	ptrScene->prerender();

	// render scene to textured
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _width, _height);
	ptrScene->render(V, P);
}

glm::mat4 Herve::DisplayDeviceMono::getOrientation() {
	return _camera.getOrientation();
}

glm::mat4 Herve::DisplayDeviceMono::getTranslation() {
	return _camera.getTranslation();
}



//////////////////////////////////////////////////////////////////////
// Stereo
//////////////////////////////////////////////////////////////////////

static const std::string vsQuadDistortion =
	"#version 330 core\n"
	"layout(location = 0) in vec3 _vertexPosition;"
	"layout(location = 1) in vec2 _vertexTexCoords;"
	"out vec2 _texCoords;"
	"void main(){"
	"	gl_Position = vec4(_vertexPosition, 1.f);"
	"	_texCoords = _vertexTexCoords;"
	"}\n";

static const std::string fsQuadDistortion =
	"#version 330 core\n"
	"in vec2 _texCoords;"	            // in [0, 1]^2 
	"out vec3 _color;"
	"uniform sampler2D _texture;"
	"uniform vec2 LensCenter;"
	"uniform vec2 Scale;"
	"uniform vec4 HmdWarpParam;"	   // in [-1, 1]^2 
	"void main() {"
	"	vec2  theta = (_texCoords - LensCenter) * 2;"
	"	float rSq = theta.x * theta.x + theta.y * theta.y;"
	"	vec2  theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + "
	"		HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);"
	"	vec2 tc = LensCenter + Scale * theta1 * 0.5;"
	"	if (!all(equal(clamp(tc, vec2(0, 0), vec2(1,1)), tc)))"
	"		_color = vec3(0);"
	"	else"
	"		_color = texture( _texture, tc).rgb;"
	"}\n";


//////////////////////////////////////////////////////////////////////
// DisplayDeviceStereo
//////////////////////////////////////////////////////////////////////

DisplayDeviceStereo::DisplayDeviceStereo(int fboWidth, int fboHeight) :
	_fboWidth(fboWidth), _fboHeight(fboHeight) {
}

void DisplayDeviceStereo::initDisplay(int maxWidth, int maxHeight) {

	DisplayDevice::initDisplay(maxWidth, maxHeight);

	assert(_fboWidth > 0);
	assert(_fboHeight > 0);
	_leftFbo.init(_fboWidth, _fboHeight);
	_rightFbo.init(_fboWidth, _fboHeight);

	// Mesh for quad rendering
	_quadMesh.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferData[] = {
			 1.0f,  -1.0f,   0.0f,
			 1.0f,   1.0f,   0.0f,
			-1.0f,  -1.0f,   0.0f,
			-1.0f,   1.0f,   0.0f
	};
	_quadMesh.pushVertexBuffer(3, vertexBufferData);
	GLfloat uvBufferData[] = {
			1.0f,   0.0f,
			1.0f,    1.0f,
			0.0f,   0.0f,
			0.0f,    1.0f
	};
	_quadMesh.pushVertexBuffer(2, uvBufferData);

	// Shader for quad rendering
	_quadShader.initFromString(vsQuadDistortion, fsQuadDistortion);

	_quadShader.bind();
	_quadShader.uniform2f("Scale", _distortionScale, _distortionScale);
	_quadShader.uniform4f("HmdWarpParam", _distortionK[0], _distortionK[1],	_distortionK[2], _distortionK[3]);
	_quadShader.unbind();
}

void DisplayDeviceStereo::render(Scene * ptrScene) {

	// prerender the scene (render framebuffers...)
	ptrScene->prerender();

	int viewportHeight;
	int viewportWidth2;
	float lensCenterXLeft;
	float lensCenterXRight;
	renderFbo(ptrScene, viewportHeight, viewportWidth2, lensCenterXLeft, lensCenterXRight);

	// render textures to screen
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_quadShader.bind();
	// left eye
	_quadShader.uniform2f("LensCenter", lensCenterXLeft, 0.5f);
	glViewport(0, _height-viewportHeight, viewportWidth2, viewportHeight);
	_leftFbo.bindTexture();
	_quadMesh.draw();
	_leftFbo.unbindTexture();
	// right eye
	_quadShader.uniform2f("LensCenter", lensCenterXRight, 0.5f);
	glViewport(viewportWidth2, _height-viewportHeight, viewportWidth2, viewportHeight);
	_rightFbo.bindTexture();
	_quadMesh.draw();
	_rightFbo.unbindTexture();
	// unbind shader
	_quadShader.unbind();
}


//////////////////////////////////////////////////////////////////////
// DisplayDeviceStereoFake
//////////////////////////////////////////////////////////////////////

DisplayDeviceStereoFake::DisplayDeviceStereoFake(int fboWidth, int fboHeight) :
		DisplayDeviceStereo(fboWidth, fboHeight) {
}

bool DisplayDeviceStereoFake::initDevice() {

	// default size
	_width = _maxWidth = 800;
	_height = _maxHeight = 600;

	_distortionScale = 0.7f;
	_distortionK[0] = 1.f;
	_distortionK[1] = 0.2f;
	_distortionK[2] = 0.1f;
	_distortionK[3] = 0.f;

	return true;
}

void DisplayDeviceStereoFake::updateView(float tx, float ty, float tz, float rx, float ry, float rz) {
	_camera.updateView(tx, ty, tz, rx, ry, rz);
}

void DisplayDeviceStereoFake::reset(float tx, float ty, float tz, float rx, float ry, float rz)
{
	_camera.reset(tx,  ty,  tz,  rx,  ry,  rz);
	std::cout<<"reset ok"<<std::endl;
}

void DisplayDeviceStereoFake::renderFbo(Scene * ptrScene, int & viewportHeight, int & viewportWidth2,
		float & lensCenterXLeft, float & lensCenterXRight) {

	lensCenterXLeft = 0.6;
	lensCenterXRight = 0.4;
	viewportHeight =_height;
	viewportWidth2 = 0.5 * _width;
	// render each eye to its texture
	renderEyeToFbo(ptrScene, -0.1, _leftFbo, viewportWidth2, viewportHeight);
	renderEyeToFbo(ptrScene, 0.1, _rightFbo, viewportWidth2, viewportHeight);
}

void DisplayDeviceStereoFake::renderEyeToFbo(Scene * ptrScene, float tx, FrameBuffer& fbo, int w, int h) {

	// projection-view matrix
	glm::mat4 V = glm::translate(glm::mat4(1.f), glm::vec3(-tx, 0.f, 0.f))
		* _camera.getOrientation() * _camera.getTranslation();

	// FIXME : il y a une rotation sur z avec glm > 0.9.3 et fov < 50
	glm::mat4 P = glm::rotate(glm::mat4(1.f), float(M_PI), glm::vec3(0.f, 0.f, 1.f))
		* glm::perspective(80.f, _width/float(_height), 0.1f, 100.f);

	// render scene to texture
	fbo.bindFrameBuffer();
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _fboWidth, _fboHeight);
	ptrScene->render(V, P);
	fbo.unbindFrameBuffer();
}

glm::mat4 Herve::DisplayDeviceStereoFake::getOrientation() {
	return _camera.getOrientation();
}

glm::mat4 Herve::DisplayDeviceStereoFake::getTranslation() {
	return _camera.getTranslation();
}

