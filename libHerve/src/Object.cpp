// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Object.hpp>
#include <Herve/Util.hpp>

using namespace Herve;

const glm::mat4& Object::getModelMatrix() const {
	return _modelMatrix;
}

Object::Object(const glm::mat4& modelMatrix, Mesh * ptrMesh, Material * ptrMaterial) :
	_modelMatrix(modelMatrix), _ptrMesh(ptrMesh), _ptrMaterial(ptrMaterial) {
}

const Mesh* Object::getPtrMesh() const {
	return _ptrMesh;
}

const Material* Object::getPtrMaterial() const {
	return _ptrMaterial;
}
