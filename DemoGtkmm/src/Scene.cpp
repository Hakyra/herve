// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

#include <cmath>

Scene::Scene(const std::string& name) : _sceneName(name) {
}

void Scene::init() {

	_sceneShader.initFromFile("vsPhongMvp.glsl", "fsPhongMvp.glsl");

	_lightColor = glm::vec3(8, 8, 8);

	_pathLight.addKey(-1, glm::vec3(-2,  0, -16));
	_pathLight.addKey( 0, glm::vec3( 2,  0, -14));
	_pathLight.addKey( 1, glm::vec3( 2,  0, -16));
	_pathLight.addKey( 2, glm::vec3(-2,  0, -14));
	// repeat the first 3 keys
	_pathLight.addKey( 3, glm::vec3(-2,  0, -16));
	_pathLight.addKey( 4, glm::vec3( 2,  0, -14));
	_pathLight.addKey( 5, glm::vec3( 2,  0, -16));

	Herve::LoaderOBJ loader;
	loader.loadOBJ("", _sceneName, glm::mat4(1.f), _uptrObjects, _uptrMeshes, _uptrTextures, _uptrMaterials);

	_chrono.start();
}

void Scene::render(const glm::mat4 &V, const glm::mat4 &P) {

	glClearColor(0.6, 0.7, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////////////////////////////////////////////////////////////
	// animation
	////////////////////////////////////////////////////////////////////////
	double time = _chrono.elapsedRunning();
	double duration = _pathLight.getEndTime(); // warning : assumes getStartTime() == 0
	int repetitions = int(time/duration);
	time -= repetitions*duration;
	_lightPosition = _pathLight.getValue(time);


	////////////////////////////////////////////////////////////////////////
	// Objects
	////////////////////////////////////////////////////////////////////////
	_sceneShader.bind();
	_sceneShader.uniform3f("_lightColor", _lightColor[0], _lightColor[1], _lightColor[2]);
	_sceneShader.uniform3f("_lightPosition_worldspace", _lightPosition[0], _lightPosition[1], _lightPosition[2]);
	_sceneShader.uniformMatrix4fv("_P", &P[0][0]);
	_sceneShader.uniformMatrix4fv("_V", &V[0][0]);

	for (auto & ptrObject : _uptrObjects) {

		// load model matrix
		glm::mat4 M = ptrObject->getModelMatrix();
		_sceneShader.uniformMatrix4fv("_M", &M[0][0]);

		// load material
		const Herve::Material * ptrMaterial = ptrObject->getPtrMaterial();
		_sceneShader.uniform3f("_ambientColor", ptrMaterial->_ka[0], ptrMaterial->_ka[1], ptrMaterial->_ka[2]);
		_sceneShader.uniform3f("_diffuseColor", ptrMaterial->_kd[0],ptrMaterial->_kd[1], ptrMaterial->_kd[2]);
		_sceneShader.uniform3f("_specularColor", ptrMaterial->_ks[0],ptrMaterial->_ks[1], ptrMaterial->_ks[2]);
		_sceneShader.uniform1f("_specularIndex", ptrMaterial->_ns);

		// bind texture
		if (ptrMaterial->_ptrTexture) {
			ptrMaterial->_ptrTexture->bind();
			_sceneShader.uniform1i("_useTexture", 1);
		}
		else {
			_sceneShader.uniform1i("_useTexture", 0);
		}

		// draw mesh
		ptrObject->getPtrMesh()->draw();
	}

	_sceneShader.unbind();
}


