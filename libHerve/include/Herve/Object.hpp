// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_OBJECT_HPP_
#define HERVE_OBJECT_HPP_

#include <Herve/Mesh.hpp>
#include <Herve/Material.hpp>

namespace Herve {

/// \brief Object = model matrix + mesh pointer + material pointer
///
class Object {

	glm::mat4 _modelMatrix;

	Mesh * _ptrMesh;
	Material * _ptrMaterial;

public:
	/// \brief construct an object
	///
	Object(const glm::mat4 & modelMatrix, Mesh * ptrMesh, Material * ptrMaterial);

	/// \brief get object model matrix
	///
	const glm::mat4 & getModelMatrix() const;

	/// \brief get mesh pointer of the object
	///
	const Mesh * getPtrMesh() const;

	/// \brief get material pointer of the object
	///
	const Material * getPtrMaterial() const;
};

} // namespace Herve

#endif
