// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SCENE_HPP_
#define HERVE_SCENE_HPP_

#include <Herve/Object.hpp>

namespace Herve {

/// \brief abstract class for implementing VR scenes
///
class Scene {
public:

	/// \brief init VR scene
	///
	/// called once, before entering main loop
	/// use this to load shaders, objects, textures, sounds...
	virtual void init();

	/// \brief pre-render the scene
	///
	/// called at every frame rendering (before render())
	/// use this to update scene data: render-to-texture, 3D sound, video stream...
	virtual void prerender();

	/// \brief render the scene
	///
	/// called at every frame rendering
	/// use this to render the scene (bind shaders/textures, draw objects...)
	virtual void render(const glm::mat4 &V, const glm::mat4 &P) = 0;
};

} // namespace Herve

#endif
