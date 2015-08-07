// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef SCENEOVR_HPP_
#define SCENEOVR_HPP_

#include <Herve/Herve.hpp>

// another scene (using render-to-texture)
class ScenePoints : public Herve::Scene {

	// dynamic texture
	Herve::FrameBuffer _textureFramebuffer;
	Herve::Mesh _textureMesh;
	Herve::Shader _textureShader;

	// main scene
	Herve::Shader _sceneShader;
	Herve::VectorUptr<Herve::Object> _objects;
	Herve::VectorUptr<Herve::Mesh> _meshes;
	Herve::MapUptr<Herve::Texture> _textures;
	Herve::MapUptr<Herve::Material> _materials;

	Herve::Chrono _chrono;

public:
	void init();
	void prerender() ;
	void render(const glm::mat4 &V, const glm::mat4 &P) ;

	float _timeStep;
	float _speed;
	int _pointsParUnite;
	float _seuilPoints;
};

#endif
