// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

void Scene::init(Herve::DisplayDevice * ptrDisplayDevice) {

	_ptrDisplayDevice = ptrDisplayDevice;

	_sceneShader.initFromFile("vsScene.glsl", "fsScene.glsl");

	Herve::LoaderOBJ loader;
	loader.loadOBJ("", "scene_sibenik.obj", glm::mat4(1.f), _uptrObjects, _uptrMeshes, _uptrTextures, _uptrMaterials);

	_pathLight.addKey(-2, glm::vec3(-3,  0,   0));
	_pathLight.addKey( 0, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 2, glm::vec3(-3, 10, -20));
	_pathLight.addKey( 4, glm::vec3( 3, 10, -20));
	_pathLight.addKey( 6, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 8, glm::vec3(-3,  0,   0));
	// repeat the first 3 keys for looping
	_pathLight.addKey( 10, glm::vec3( 3,  0,   0));
	_pathLight.addKey( 12, glm::vec3( 0,  0, -10));
	_pathLight.addKey( 14, glm::vec3(-3, 10, -20));
	_chrono.start();

	_videoMeshLeft.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferData[] = {
			 3.f,  -2.f,  0.f,
			 3.f,   2.f,  0.f,
			-3.f,  -2.f,  0.f,
			-3.f,   2.f,  0.f
	};
	_videoMeshLeft.pushVertexBuffer(3, vertexBufferData);
	GLfloat normalBufferData[] = {
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f,
			0.f,  0.f,  1.f
	};
	_videoMeshLeft.pushVertexBuffer(3, normalBufferData);
	GLfloat uvBufferData[] = {
			1.0f,  1.0f,
			1.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  0.0f
	};
	_videoMeshLeft.pushVertexBuffer(2, uvBufferData);

	_videoMeshRight.initMesh(GL_TRIANGLE_STRIP, 4);
	GLfloat vertexBufferData2[] = {
			 3.f,  -2.f,  0.f,
			 3.f,   2.f,  0.f,
			-3.f,  -2.f,  0.f,
			-3.f,   2.f,  0.f
	};
	_videoMeshRight.pushVertexBuffer(3, vertexBufferData2);
	GLfloat normalBufferData2[] = {
			0.f,  0.f,  -1.f,
			0.f,  0.f,  -1.f,
			0.f,  0.f,  -1.f,
			0.f,  0.f,  -1.f
	};
	_videoMeshRight.pushVertexBuffer(3, normalBufferData2);
	GLfloat uvBufferData2[] = {
			1.0f,  1.0f,
			1.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  0.0f
	};
	_videoMeshRight.pushVertexBuffer(2, uvBufferData2);

	_videoTexture.init("05.mp4", 640, 360);
	_videoTexture.play();


	Herve::Sound::initAudio();
	Herve::SoundFx::initFx();
	_music.init("song47.ogg");
	_music.play();
	_hasMusic = true;
	_sound.init("sirene_pompiers.ogg");
	_soundFxReverb.init();
	_soundFxReverb.connectSound(_sound);
	_sound.play();
	_sound.setLooping(AL_TRUE);
	_hasSound = true;

	_hasSound3d = false;
	_hasSoundDoppler = false;
	_hasSoundReverb = false;

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
	// render audio
	if (_hasSoundReverb) _soundFxReverb.connectSound(_sound);
	else _soundFxReverb.disconnectSound(_sound);
	glm::vec3 soundPosition;
	glm::vec3 soundVelocity;
	getRelativePositionAndVelocity(soundPosition, soundVelocity);
	if (_hasSound3d) _sound.setPosition(soundPosition[0], soundPosition[1], soundPosition[2]);
	else _sound.setPosition(0, 0, 0);
	if (_hasSoundDoppler) _sound.setVelocity(soundVelocity[0], soundVelocity[1], soundVelocity[2]);
	else _sound.setVelocity(0, 0, 0);

	_videoTexture.update();
}

void Scene::render(const glm::mat4 &V, const glm::mat4 &P) {

	// render display
	glm::vec3 lightPosition = _pathLight.getValue(getTime());
	_sceneShader.bind();
	_sceneShader.uniform3f("_lightPosition_worldspace", lightPosition[0], lightPosition[1], lightPosition[2]);
	_sceneShader.uniform3f("_lightColor", 20.f, 20.f, 20.f);
	const glm::mat4 VP = P * V;
	_sceneShader.uniformMatrix4fv("_VP", &VP[0][0]);

	// render main scene
	for (auto & ptrObject : _uptrObjects) {

		// load model matrix
		glm::mat4 M = ptrObject->getModelMatrix();
		_sceneShader.uniformMatrix4fv("_M", &M[0][0]);

		// load material
		const Herve::Material * ptrMaterial = ptrObject->getPtrMaterial();
		_sceneShader.uniform3f("_ambientColor",
				ptrMaterial->_ka[0], ptrMaterial->_ka[1], ptrMaterial->_ka[2]);
		_sceneShader.uniform3f("_diffuseColor",
				ptrMaterial->_kd[0], ptrMaterial->_kd[1], ptrMaterial->_kd[2]);

		// bind texture
		if (ptrMaterial->_ptrTexture) {
			ptrMaterial->_ptrTexture->bind();
			_sceneShader.uniform1i("_useTexture", 1);
			_sceneShader.uniform1i("_useUv", 1);
		}
		else {
			_sceneShader.uniform1i("_useTexture", 0);
		}

		// draw mesh
		ptrObject->getPtrMesh()->draw();
		Herve::Texture::unbind();
	}


	// render video textures
	_sceneShader.uniform3f("_ambientColor", 0.f, 0.f, 0.f);
	_sceneShader.uniform3f("_diffuseColor", 0.8f, 0.8f, 0.8f);
	_sceneShader.uniform1i("_useTexture", 1);
	_sceneShader.uniform1i("_useUv", 1);

	glm::mat4 M(1.f);

	// first video texture
	M = glm::rotate(glm::mat4(1.f), float(0.5*M_PI), glm::vec3(0.f, 1.f, 0.f))
		* glm::translate(glm::mat4(1.f), glm::vec3(7.f, 0.f, -7.f));
	_sceneShader.uniformMatrix4fv("_M", &M[0][0]);
	_videoTexture.bind();
	_videoMeshLeft.draw();
	Herve::Texture::unbind();

	// second video texture
	M = glm::translate(glm::mat4(1.f), glm::vec3(5.f, 0.f, -6.f))
		* glm::rotate(glm::mat4(1.f), float(0.5*M_PI), glm::vec3(0.f, 1.f, 0.f));
	_sceneShader.uniformMatrix4fv("_M", &M[0][0]);
	_videoTexture.bind();
	_videoMeshRight.draw();
	Herve::Texture::unbind();

	_sceneShader.unbind();
}

Scene::~Scene() {
	_music.free();
	_sound.free();
	_soundFxReverb.free();
	Herve::Sound::freeAudio();
}

float Scene::getTime() {
	double time = _chrono.elapsedRunning();
	double duration = _pathLight.getEndTime(); // assumes getStartTime() == 0
	int repetitions = int(time/duration);
	time -= repetitions*duration;
	return time;
}

bool Scene::getHasMusic() { return _hasMusic; }
bool Scene::getHasSound() { return _hasSound; }
bool Scene::getHasSound3d() { return _hasSound3d; }
bool Scene::getHasSoundDoppler() { return _hasSoundDoppler; }
bool Scene::getHasSoundReverb() { return _hasSoundReverb; }

void Scene::switchMusic() {
	_hasMusic = not _hasMusic;
	if (_hasMusic) _music.play();
	else _music.pause();
}

void Scene::switchSound() {
	_hasSound = not _hasSound;
	if (_hasSound) _sound.play();
	else _sound.pause();
}

void Scene::switchSound3d() { _hasSound3d = not _hasSound3d; }
void Scene::switchSoundDoppler() { _hasSoundDoppler = not _hasSoundDoppler; }
void Scene::switchSoundReverb() { _hasSoundReverb = not _hasSoundReverb; }


