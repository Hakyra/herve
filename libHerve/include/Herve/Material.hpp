// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_MATERIAL_HPP_
#define HERVE_MATERIAL_HPP_

#include <Herve/glm.hpp>
#include <Herve/Texture.hpp>

namespace Herve {

/// \brief Phong material
///
struct Material {

	/// \brief ambient color
	///
	glm::vec3 _ka;

	/// \brief diffuse color
	///
	glm::vec3 _kd;

	/// \brief specular color
	///
	glm::vec3 _ks;

	/// \brief specular index
	///
	float _ns;

	/// \brief diffuse texture (or NULL if no texture)
	///
	Texture * _ptrTexture;

	/// \brief construct a default material
	///
	Material();
};

} // namespace Herve

#endif
