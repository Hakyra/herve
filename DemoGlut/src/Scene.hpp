// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <Herve/Herve.hpp>

// scene: sibenik 3D model + 3D sound + video texture + moving light/audio source
class Scene : public Herve::Scene {

	Herve::Shader _sceneShader;

	Herve::VectorUptr<Herve::Object> _uptrObjects;

	Herve::VectorUptr<Herve::Mesh> _uptrMeshes;
	Herve::MapUptr<Herve::Texture> _uptrTextures;
	Herve::MapUptr<Herve::Material> _uptrMaterials;
	
	Herve::Chrono _chrono;
	Herve::Spline3<glm::vec3, float> _pathLight;

	Herve::Sound _music;
	Herve::Sound _sound;
	Herve::SoundFxReverb _soundFxReverb;

	bool _hasMusic;
	bool _hasSound;
	bool _hasSound3d;
	bool _hasSoundDoppler;
	bool _hasSoundReverb;

	Herve::DisplayDevice * _ptrDisplayDevice;

	Herve::Mesh _videoMeshLeft;
	Herve::Mesh _videoMeshRight;
	Herve::TextureVideo _videoTexture;

public:
	void init(Herve::DisplayDevice * ptrDisplayDevice);

	void prerender();

	void render(const glm::mat4 &V, const glm::mat4 &P);

	// get light/audio source position an velocity
	void getRelativePositionAndVelocity(glm::vec3 & position, glm::vec3 & velocity);

	float getTime();

	bool getHasMusic();
	bool getHasSound();
	bool getHasSound3d();
	bool getHasSoundDoppler();
	bool getHasSoundReverb();

	void switchMusic();
	void switchSound();
	void switchSound3d();
	void switchSoundDoppler();
	void switchSoundReverb();

	~Scene();
};

#endif
