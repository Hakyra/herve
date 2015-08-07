// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

void Scene::init(Herve::DisplayDevice * ptrDisplayDevice) {

	// init display device
	_ptrDisplayDevice = ptrDisplayDevice;

	// init shaders for quad
	_sceneShader.initFromFile("vsDemo.glsl", "fsDemo.glsl");

	// init quad mesh
	_quadMesh.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferData[] = {
			 1.0f,  -1.0f,   0.0f,
			 1.0f,   1.0f,   0.0f,
			-1.0f,  -1.0f,   0.0f,
			-1.0f,   1.0f,   0.0f
	};
	_quadMesh.pushVertexBuffer(3, vertexBufferData);
	GLfloat normalBufferData[] = {
			 0.0f,   0.0f,   1.0f,
			 0.0f,   0.0f,   1.0f,
			 0.0f,   0.0f,   1.0f,
			 0.0f,   0.0f,   1.0f
	};
	_quadMesh.pushVertexBuffer(3, normalBufferData);
	GLfloat uvBufferData[] = {
			1.0f,   0.0f,
			1.0f,    1.0f,
			0.0f,   0.0f,
			0.0f,    1.0f
	};
	_quadMesh.pushVertexBuffer(2, uvBufferData);
}


void Scene::prerender() {
}

void Scene::render(const glm::mat4 &V, const glm::mat4 &P) {

	// clear frame buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw scene
	_sceneShader.bind();
	_quadMesh.draw();
	_sceneShader.unbind();

}


