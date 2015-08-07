// Copyright �� 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

#include <cmath>

void Scene::init(Herve::DisplayDevice * ptrDisplayDevice) {

	_ptrDisplayDevice = ptrDisplayDevice;

	_sceneShader.initFromFile("vsScene.glsl", "fsScene.glsl");

	_lightColor = glm::vec3(12, 12, 12);

	_pathLight.addKey(-1, glm::vec3(-3,  0,   0));
	_pathLight.addKey( 0, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 1, glm::vec3(-3, 10, -20));
	_pathLight.addKey( 2, glm::vec3( 3, 10, -20));
	_pathLight.addKey( 3, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 4, glm::vec3(-3,  0,   0));
	_pathLight.addKey( 5, glm::vec3( 3,  0,   0));
	_pathLight.addKey( 6, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 7, glm::vec3(-3, 10, -20));

	Herve::LoaderOBJ loader;
	loader.loadOBJ("./sibenik/", "sibenik_lux.obj", glm::mat4(1.f), _uptrObjects, _uptrMeshes, _uptrTextures, _uptrMaterials);


	_videoMesh.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferData[] = {
			 3.f,  -2.f,  0.f,
			 3.f,   2.f,  0.f,
			-3.f,  -2.f,  0.f,
			-3.f,   2.f,  0.f
	};
	_videoMesh.pushVertexBuffer(3, vertexBufferData);
	GLfloat normalBufferData[] = {
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f
	};
	_videoMesh.pushVertexBuffer(3, normalBufferData);
	GLfloat uvBufferData[] = {
			1.0f,  1.0f,
			1.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  0.0f
	};
	_videoMesh.pushVertexBuffer(2, uvBufferData);

	_videoTexture.init("video/05.mp4", 640, 360);
	_videoTexture.play();


	Herve::Sound::initAudio();
	Herve::SoundFx::initFx();
	//_music.init("audio/song47.ogg");
	//_music.play();
	//_sound.init("audio/sirene_pompiers.ogg");
	_sound.init("audio/song47.ogg");
	//_soundFxReverb.init();
	//_soundFxReverb.connectSound(_sound);
	_sound.play();

	_chrono.start();
}

float Scene::getTime() {
	double time = _chrono.elapsedRunning();
	double duration = _pathLight.getEndTime(); // assumes getStartTime() == 0
	int repetitions = int(time/duration);
	time -= repetitions*duration;
	return time;
}

void Scene::getRelativePositionAndVelocity(glm::vec3 & position, glm::vec3 & velocity) {

	// animation
	glm::vec3 p(0.f, 0.f, 0.f);
	glm::vec3 v(0.f, 0.f, 0.f);
	_pathLight.getValueAndDerivative(getTime(), p, v);

	// source position
	glm::mat4 relativePosition = _ptrDisplayDevice->getOrientation() * _ptrDisplayDevice->getTranslation()
			*  glm::translate(glm::mat4(1.f), p);
	position = glm::vec3(relativePosition[3][0], relativePosition[3][1], relativePosition[3][2]);

	glm::mat4 relativeVelocity = _ptrDisplayDevice->getOrientation() *  glm::translate(glm::mat4(1.f), v);
	velocity = glm::vec3(relativeVelocity[3][0], relativeVelocity[3][1], relativeVelocity[3][2]);
}

void Scene::prerender() {

	// 3D audio
	glm::vec3 soundPosition;
	glm::vec3 soundVelocity;
	getRelativePositionAndVelocity(soundPosition, soundVelocity);
	_sound.setPosition(soundPosition[0], soundPosition[1], soundPosition[2]);
	_sound.setVelocity(soundVelocity[0], soundVelocity[1], soundVelocity[2]);

	// video texture
	_videoTexture.update();
}

void Scene::render(const glm::mat4 &V, const glm::mat4 &P) {

	////////////////////////////////////////////////////////////////////////
	// animation
	////////////////////////////////////////////////////////////////////////

	double time = _chrono.elapsedRunning();
	double duration = _pathLight.getEndTime(); // warning : assumes getStartTime() == 0
	int repetitions = int(time/duration);
	time -= repetitions*duration;
	glm::vec3 lightPosition = _pathLight.getValue(time);


	////////////////////////////////////////////////////////////////////////
	// Rendering
	////////////////////////////////////////////////////////////////////////

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_sceneShader.bind();
	_sceneShader.uniform3f("_lightColor", _lightColor[0], _lightColor[1], _lightColor[2]);
	_sceneShader.uniform3f("_lightPosition_worldspace", lightPosition[0], lightPosition[1], lightPosition[2]);
	_sceneShader.uniformMatrix4fv("_P", &P[0][0]);
	_sceneShader.uniformMatrix4fv("_V", &V[0][0]);

	// Objects from OBJ
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

	// video
	_sceneShader.uniform3f("_ambientColor", 0.f, 0.f, 0.f);
	_sceneShader.uniform3f("_diffuseColor", 1.f, 1.f, 1.f);
	_sceneShader.uniform3f("_specularColor", 0.f, 0.f, 0.f);
	_sceneShader.uniform1f("_specularIndex", 1.f);
	_sceneShader.uniform1i("_useTexture", 1);
	_sceneShader.uniform1i("_useUv", 1);
	glm::mat4 M(1.f);

	M = glm::rotate(glm::mat4(1.f), float(0.5*M_PI), glm::vec3(0.f, 1.f, 0.f))
		* glm::translate(glm::mat4(1.f), glm::vec3(7.f, 0.f, -7.f));
	// FIXME debian
	/*
	M = glm::rotate(glm::mat4(1.f), glm::degrees(float(0.5*M_PI)), glm::vec3(0.f, 1.f, 0.f))
		* glm::translate(glm::mat4(1.f), glm::vec3(7.f, 0.f, -7.f));
	*/
	_sceneShader.uniformMatrix4fv("_M", &M[0][0]);
	_videoTexture.bind();
	_videoMesh.draw();
	Herve::Texture::unbind();

	_sceneShader.unbind();
}


