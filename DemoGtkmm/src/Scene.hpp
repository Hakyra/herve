// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <Herve/Herve.hpp>

// 3D scene loaded from OBJ file (wavefront file format).
// Model in MVC pattern
class Scene : public Herve::Scene {

	std::string _sceneName;

	Herve::Chrono _chrono;

	Herve::Spline3<glm::vec3, float> _pathLight;

	Herve::Shader _sceneShader;

	glm::vec3 _lightColor;
	glm::vec3 _lightPosition;

	Herve::VectorUptr<Herve::Object> _uptrObjects;
	Herve::VectorUptr<Herve::Mesh> _uptrMeshes;
	Herve::MapUptr<Herve::Texture> _uptrTextures;
	Herve::MapUptr<Herve::Material> _uptrMaterials;

public:
	Scene(const std::string & name);
	void init();
	void render(const glm::mat4 &V, const glm::mat4 &P);
};

#endif
