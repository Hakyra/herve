// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <Herve/Herve.hpp>

// scene: sibenik 3D model + 3D sound + video texture + moving light/audio source
class Scene : public Herve::Scene {

	Herve::DisplayDevice * _ptrDisplayDevice;

	Herve::Shader _sceneShader;
	Herve::VectorUptr<Herve::Object> _uptrObjects;
	Herve::VectorUptr<Herve::Mesh> _uptrMeshes;
	Herve::MapUptr<Herve::Texture> _uptrTextures;
	Herve::MapUptr<Herve::Material> _uptrMaterials;

	Herve::Chrono _chrono;
	Herve::Spline3<glm::vec3, float> _pathLight;
	glm::vec3 _lightColor;

	Herve::Mesh _videoMesh;
	Herve::TextureVideo _videoTexture;

	Herve::Sound _music;
	Herve::Sound _sound;
	Herve::SoundFxReverb _soundFxReverb;

public:

	void init(Herve::DisplayDevice * ptrDisplayDevice);
	void prerender();
	void render(const glm::mat4 &V, const glm::mat4 &P) ;

private:
	float getTime();
	void getRelativePositionAndVelocity(glm::vec3 & position, glm::vec3 & velocity);
};

#endif
