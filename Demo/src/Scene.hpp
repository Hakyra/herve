// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <Herve/Herve.hpp>

// the scene of the VR application: 3D objects, sounds, animation...
class Scene : public Herve::Scene {

	// display device (to render the scene)
	Herve::DisplayDevice * _ptrDisplayDevice;

	// a simple scene: quad + gradient color (uv)
	Herve::Shader _sceneShader;
	Herve::Mesh _quadMesh;

public:

	// init the scene
	// called once, before entering main loop
	// use this to load shaders, objects, textures, sounds...
	void init(Herve::DisplayDevice * ptrDisplayDevice);

	// pre-render the scene
	// called at every frame rendering (before render())
	// use this to update scene data: render-to-texture, 3D sound, video stream...
	void prerender();

	// render the scene
	// called at every frame rendering
	// use this to render the scene (bind shaders/textures, draw objects...)
	void render(const glm::mat4 &V, const glm::mat4 &P) ;
};

#endif
