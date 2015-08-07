// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "SceneOvr.hpp"

#include <cmath>
#include <iostream>
#include <string>

static const std::string vsTexture =
		"#version 330 core\n"
		"layout(location = 0) in vec3 _vertexPosition_modelspace;"
		"layout(location = 1) in vec2 _vertexUv;"
		"out vec2 _uv;"
		"void main() {"
		"    gl_Position = vec4(_vertexPosition_modelspace, 1.f);"
		"    _uv = _vertexUv;"
		"}\n";

static const std::string fsTexture =
		"#version 330 core\n"
		"out vec3 _color;"
		"in vec2 _uv;"
		"uniform int _pointsParUnite;"
		"uniform float _rayonPoints;"
		"void main() {"
		"	vec2 d0 = _uv * _pointsParUnite;"
		"	vec2 d1 = vec2(round(d0));"
		"	if (distance(d0, d1) < _rayonPoints)"
		"		_color = vec3(1);"
		"	else"
		"		_color = vec3(0);"
		"}\n";

static const std::string vsScene =
		"#version 330 core\n"
		"layout(location = 0) in vec3 _vertexPosition_modelspace;"
		"layout(location = 1) in vec2 _vertexUv;"
		"uniform mat4 _P;"
		"uniform mat4 _V;"
		"out vec2 _uv;"
		"void main() {"
		"    gl_Position = _P *_V * vec4(_vertexPosition_modelspace, 1.f);"
		"    _uv = _vertexUv;"
		"}\n";

static const std::string fsScene =
		"#version 330 core\n"
		"layout(location = 0) out vec3 _fragmentColor;"
		"in vec2 _uv;"
		"uniform vec2 _duv;"
		"uniform sampler2D _texture;"
		"void main() {"
		"	_fragmentColor = texture( _texture, _uv+_duv).rgb;"
		"}\n";

void ScenePoints::init() {

	// dynamic texture
	_timeStep = 5;
	_speed = 0.1;
	_pointsParUnite = 3;
	_seuilPoints = 0.1;
	_textureFramebuffer.init(1024, 1024);
	_textureMesh.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferDataTexture[] = {
			 1.0f,  -1.0f,   0.0f,
			 1.0f,   1.0f,   0.0f,
			-1.0f,  -1.0f,   0.0f,
			-1.0f,   1.0f,   0.0f
	};
	_textureMesh.pushVertexBuffer(3, vertexBufferDataTexture);
	GLfloat uvBufferDataTexture[] = {
			1.0f,   0.0f,
			1.0f,    1.0f,
			0.0f,   0.0f,
			0.0f,    1.0f
	};
	_textureMesh.pushVertexBuffer(2, uvBufferDataTexture);
	_textureShader.initFromString(vsTexture, fsTexture);


	// main scene
	_sceneShader.initFromString(vsScene, fsScene);
	Herve::LoaderOBJ loader;
	loader.loadOBJ("", "screen.obj", glm::mat4(1.f), _objects, _meshes, _textures, _materials);


	_chrono.start();
}


void ScenePoints::prerender() {

	// render the dynamic texture (used in main scene later)
	_textureFramebuffer.bindFrameBuffer();
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _textureFramebuffer.getWidth(), _textureFramebuffer.getHeight());
	_textureShader.bind();
	_textureShader.uniform1i("_pointsParUnite", _pointsParUnite);
	_textureShader.uniform1f("_rayonPoints", _seuilPoints);
	_textureMesh.draw();
	_textureShader.unbind();
	_textureFramebuffer.unbindFrameBuffer();

}

void ScenePoints::render(const glm::mat4 &V, const glm::mat4 &P) {

	// render main scene
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_sceneShader.bind();
	_sceneShader.uniformMatrix4fv("_P", &P[0][0]);
	_sceneShader.uniformMatrix4fv("_V", &V[0][0]);

	float time = _chrono.elapsedRunning();
	int q = time / (7*_timeStep);
	time -= q * (7*_timeStep);
	float du, dv;
	// no animation
	if (time < _timeStep) {
		du = 0;
		dv = 0;
	}
	// down
	else if (time < 2*_timeStep) {
		du = 0;
		dv = -time*_speed;
	}
	// up
	else if (time < 3*_timeStep) {
		du = 0;
		dv = time*_speed;
	}
	// left
	else if (time < 4*_timeStep) {
		du = -time*_speed;
		dv = 0;
	}
	// right
	else if (time < 5*_timeStep) {
		du = time*_speed;
		dv = 0;
	}
	// up left
	else if (time < 6*_timeStep) {
		du = -time*_speed;
		dv = time*_speed;
	}
	// down right
	else {
		du = time*_speed;
		dv = time*_speed;
	}
	_sceneShader.uniform2f("_duv", du, dv);

	_textureFramebuffer.bindTexture();
	_meshes.front()->draw();
	_textureFramebuffer.unbindTexture();

	_sceneShader.unbind();
}

