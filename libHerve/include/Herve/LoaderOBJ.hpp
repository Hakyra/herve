// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_LOADEROBJ_HPP_
#define HERVE_LOADEROBJ_HPP_

#include <Herve/glm.hpp>
#include <Herve/Object.hpp>
#include <Herve/Util.hpp>

#include <fstream>

namespace Herve {

/// \brief load OBJ file (mesh, material, texture from PNG file)
///
class LoaderOBJ {
public:
	/// \brief load data; OBJ, MTL and PNG must be accessible at the given path
	///
	bool loadOBJ(
			const std::string& path,
			const std::string& filename,
			const glm::mat4 & modelMatrix,
			VectorUptr<Object> & uptrObjects,
			VectorUptr<Mesh> & uptrMeshes,
			MapUptr<Texture> & uptrTextures,
			MapUptr<Material> & uptrMaterials);

private:

	// load vertex data in _vertex* and textures/materials
	void readVerticesAndMaterials(
			const std::string& path,
			const std::string & filename,
			MapUptr<Texture> & uptrTextures,
			MapUptr<Material> & uptrMaterials);

	void readMtl(
			const std::string& path,
			const std::string & mtlFilename,
			MapUptr<Texture> & uptrTextures,
			MapUptr<Material> & uptrMaterials);

	// read file till the beginning of the first object
	void readObjectInit(std::ifstream & file);

	// read one object from file and store the data in _oneObject*
	// return true if a valid object has been read
	// usage :
	//   - call readVerticesAndMaterials once
	//   - call readObjectInit once
	//   - call readObjectFetch while valid objects are read
	bool readObjectFetch(std::ifstream & file);

	// data for OBJ reading
	std::vector<glm::vec3> _vertexPositions;
	std::vector<glm::vec3> _vertexNormals;
	std::vector<glm::vec2> _vertexUvs;
	Material * _ptrDefaultMaterial;

	// data for object creation
	std::vector<GLuint> _oneObjectElements;
	std::vector<glm::vec3> _oneObjectPositions;
	std::vector<glm::vec3> _oneObjectNormals;
	std::vector<glm::vec2> _oneObjectUvs;
	std::string _oneObjectMaterialName;

	struct Vertex {
		int _index;
		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec2 _uv;
		Vertex(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec2 & uv);
		inline bool operator<(const Vertex & v) const;
	};

	struct ObjVertex { int _iPosition, _iNormal, _iUv; };

	friend std::istream & operator>>(std::istream & is, ObjVertex & vertex);
};

std::istream & operator>>(std::istream & is, LoaderOBJ::ObjVertex & vertex);

} // namespace Herve

#endif
