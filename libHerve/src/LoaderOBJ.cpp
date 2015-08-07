// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Chrono.hpp>
#include <Herve/LoaderOBJ.hpp>
#include <Herve/Util.hpp>

#include <sstream>
#include <set>

using namespace Herve;

bool LoaderOBJ::loadOBJ(
		const std::string& path,
		const std::string& filename,
		const glm::mat4 & modelMatrix,
		VectorUptr<Object> & uptrObjects,
		VectorUptr<Mesh> & uptrMeshes,
		MapUptr<Texture> & uptrTextures,
		MapUptr<Material> & uptrMaterials) {


	unsigned int nbVertices = 0;
	unsigned int nbTriangles = 0;
	unsigned int nbMeshes = 0;

	Chrono chrono;
	chrono.start();

	// read vertex data
	readVerticesAndMaterials(path, filename, uptrTextures, uptrMaterials);

	// read objects
	std::ifstream file(path + filename);
	readObjectInit(file);
	while (readObjectFetch(file)) {

		// get material (use the default material if not found)
		Material * ptrMaterial = _ptrDefaultMaterial;
		MapUptr<Material>::const_iterator iterMaterials = uptrMaterials.find(_oneObjectMaterialName);
		if (iterMaterials != uptrMaterials.end()) ptrMaterial = iterMaterials->second.get();

		// add mesh
		ElementMesh * ptrMesh = new ElementMesh();
		ptrMesh->initMesh(GL_TRIANGLES, _oneObjectPositions.size());
		ptrMesh->initElementBuffer(_oneObjectElements.size(), (GLuint*)&_oneObjectElements[0]);
		ptrMesh->pushVertexBuffer(3, (GLfloat*)&_oneObjectPositions[0]);
		ptrMesh->pushVertexBuffer(3, (GLfloat*)&_oneObjectNormals[0]);
		ptrMesh->pushVertexBuffer(2, (GLfloat*)&_oneObjectUvs[0]);
		uptrMeshes.push_back(std::unique_ptr<Mesh>(ptrMesh));

		// add object
		Object * ptrObject = new Object(modelMatrix, ptrMesh, ptrMaterial);
		uptrObjects.push_back(std::unique_ptr<Object>(ptrObject));

		// update infos
		nbVertices += _oneObjectPositions.size();
		nbTriangles += _oneObjectElements.size()/3;
		nbMeshes++;
	}

	chrono.stop();

	// print infos
	std::ostringstream oss;
	oss << " loaded (time = " << chrono.elapsedStopped() << " s, ";
	oss << "vertices = " << nbVertices << ", ";
	oss << "triangles = " << nbTriangles << ", ";
	oss << "meshes = " << nbMeshes << ", ";
	oss << "materials = " << uptrMaterials.size() << ", ";
	oss << "textures = " << uptrTextures.size() << ")";
	utilInfo(filename.c_str(),  oss.str().c_str(), __FILE__, __LINE__);

	return true;
}

void LoaderOBJ::readVerticesAndMaterials(
		const std::string& path,
		const std::string & filename,
		MapUptr<Texture> & uptrTextures,
		MapUptr<Material> & uptrMaterials) {

	// open OBJ file
	std::string fullName = path + filename;
	std::ifstream file(fullName);
	if (not file) {
		utilInfo("cannot open ", fullName.c_str(), __FILE__, __LINE__);
	}

	_vertexPositions.clear();
	_vertexNormals.clear();
	_vertexUvs.clear();

	// load data from OBJ file (vertices/normals/uvs and MTL filename)
	while (file) {

		// read first token
		std::string token;
		file >> token;

		// if "v", vertex position
		if (token == "v") {
			float x, y, z;
			// read vertex coordinates
			file >> x >> y >> z;
			// read rest of line
			std::getline(file, token, '\n');
			// add position in vector
			_vertexPositions.push_back(glm::vec3(x, y, z));
		}
		// if "vn", vertex normal
		else if (token == "vn") {
			float x, y, z;
			file >> x >> y >> z;
			std::getline(file, token, '\n');
			_vertexNormals.push_back(glm::vec3(x, y, z));
		}
		// if "vt", vertex uv
		else if (token == "vt") {
			float u, v;
			file >> u >> v;
			std::getline(file, token, '\n');
			_vertexUvs.push_back(glm::vec2(u, v));
		}
		// if "mtllib", MTL filename
		else if (token == "mtllib") {
			std::string mtlFilename;
			file >> mtlFilename;
			std::getline(file, token, '\n');
			utilInfo("mtllib : ", mtlFilename.c_str(), __FILE__, __LINE__);
			readMtl(path, mtlFilename, uptrTextures, uptrMaterials);
		}
		else {
			// read the rest of the line
			std::getline(file, token, '\n');
		}
	}

	// add a default material
	std::string nameDefautMaterial("herve_default_material");
	_ptrDefaultMaterial = new Material;
	uptrMaterials.insert(MapUptr<Material>::value_type (nameDefautMaterial, std::unique_ptr<Material>(_ptrDefaultMaterial)));

}

void LoaderOBJ::readMtl(
		const std::string& path,
		const std::string & mtlFilename,
		MapUptr<Texture> & uptrTextures,
		MapUptr<Material> & uptrMaterials) {

	// open MTL file
	std::string fullName = path + mtlFilename;
	std::ifstream file(fullName);
	if (not file) {
		utilInfo("cannot open ", fullName.c_str(), __FILE__, __LINE__);
		return;
	}

	std::string token;
	std::string materialName;
	Material * ptrMaterial = 0;

	// read material till eof
	while (file) {

		// read token
		file >> token;

		// parse token
		if (token == "Ns") {
			float ns;
			file >> ns;
			std::getline(file, token, '\n');
			if (ptrMaterial) ptrMaterial->_ns = ns;
			else utilInfo("invalid material in ", mtlFilename.c_str(), __FILE__, __LINE__);
		}
		else if (token == "Ka") {
			float r, g, b;
			file >> r >> g >> b;
			std::getline(file, token, '\n');
			if (ptrMaterial) ptrMaterial->_ka = glm::vec3(r, g, b);
			else utilInfo("invalid material in ", mtlFilename.c_str(), __FILE__, __LINE__);
		}
		else if (token == "Kd") {
			float r, g, b;
			file >> r >> g >> b;
			std::getline(file, token, '\n');
			if (ptrMaterial) ptrMaterial->_kd = glm::vec3(r, g, b);
			else utilInfo("invalid material in ", mtlFilename.c_str(), __FILE__, __LINE__);
		}
		else if (token == "Ks") {
			float r, g, b;
			file >> r >> g >> b;
			std::getline(file, token, '\n');
			if (ptrMaterial) ptrMaterial->_ks = glm::vec3(r, g, b);
			else utilInfo("invalid material in ", mtlFilename.c_str(), __FILE__, __LINE__);
		}
		else if (token == "map_Kd") {
			// get texture name
			std::string textureName;
			file >> textureName;
			std::getline(file, token, '\n');
			Texture * ptrTexture = 0;
			MapUptr<Texture>::const_iterator iterTextures = uptrTextures.find(textureName);
			if (iterTextures != uptrTextures.end()) {
				// if texture already in map, use it
				ptrTexture = iterTextures->second.get();
			}
			else {
				// else load texture and add it in map
				ptrTexture = new Texture;
				if (ptrTexture->initFromFile(path + textureName))
					uptrTextures.insert(MapUptr<Texture>::value_type (textureName, std::unique_ptr<Texture>(ptrTexture)));
			}
			ptrMaterial->_ptrTexture = ptrTexture;
		}
		else if (token == "newmtl") {
			// add current material into the map
			if (ptrMaterial)
				uptrMaterials.insert(MapUptr<Material>::value_type (materialName, std::unique_ptr<Material>(ptrMaterial)));

			// begin another material
			ptrMaterial = new Material;
			file >> materialName;
			std::getline(file, token, '\n');
		}
		else {
			std::getline(file, token, '\n');
		}
	}

	// add last material
	if (ptrMaterial)
		uptrMaterials.insert(MapUptr<Material>::value_type (materialName, std::unique_ptr<Material>(ptrMaterial)));
}

void LoaderOBJ::readObjectInit(std::ifstream & file) {

	// read file till first object
	while (file) {

		// read first token
		std::string token;
		file >> token;

		if (token == "usemtl") {
			break;
		}
		else {
			// read the rest of the line
			std::getline(file, token, '\n');
		}
	}
}

bool LoaderOBJ::readObjectFetch(std::ifstream& file) {

	// data from OBJ file
	std::vector<ObjVertex> objVertices;

	std::string token;

	// here we have read "usemtl" so we now have to read material name
	file >> _oneObjectMaterialName;
	std::getline(file, token, '\n');

	// read the file until we reach next object or eof
	while (file) {

		file >> token;

		// if "f", face
		if (token == "f") {
			ObjVertex v1, v2, v3;
			file >> v1 >> v2 >> v3;
			std::getline(file, token, '\n');
			objVertices.push_back(v1);
			objVertices.push_back(v2);
			objVertices.push_back(v3);
		}
		// if "usemtl", then stop (new object)
		else if (token == "usemtl") {
			break;
		}
		else {
			// read the rest of the line
			std::getline(file, token, '\n');
		}
	}

	if (objVertices.empty()) {
		return false;
	}

	// format data into vertex buffers
	_oneObjectElements.clear();
	_oneObjectPositions.clear();
	_oneObjectNormals.clear();
	_oneObjectUvs.clear();

	std::set<Vertex> vertexSet;
	unsigned int iLastVertex = 0;

	// for each vertex from OBJ file
	for (ObjVertex & objVertex : objVertices) {

		// build vertex and find it in vertexSet
		Vertex v(_vertexPositions[objVertex._iPosition - 1],
				_vertexNormals[objVertex._iNormal - 1],
				_vertexUvs[objVertex._iUv - 1]);
		auto vFound = vertexSet.find(v);

		// vertex in vertexSet: update _elements
		if (vFound != vertexSet.end()) {
			_oneObjectElements.push_back(vFound->_index);
		}
		// vertex not in vertexSet: update vertexSet, iLastVertex, _elements, _positions, _normals
		else {
			v._index = iLastVertex;
			vertexSet.insert(v);
			_oneObjectElements.push_back(iLastVertex);
			_oneObjectPositions.push_back(v._position);
			_oneObjectNormals.push_back(v._normal);
			_oneObjectUvs.push_back(v._uv);
			iLastVertex++;
		}
	}

	return true;
}


std::istream& Herve::operator>>(std::istream& is, LoaderOBJ::ObjVertex& vertex) {
	std::string token;
	is >> vertex._iPosition;
	std::getline(is, token, '/');
	is >> vertex._iUv;
	std::getline(is, token, '/');
	is >> vertex._iNormal;
	return is;
}

LoaderOBJ::Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv) :
								  _position(position), _normal(normal), _uv(uv) {
}

inline bool LoaderOBJ::Vertex::operator <(const Vertex& v) const {

	const float EPSILON = 1e-3;

	const glm::vec3 & p1 = _position;
	const glm::vec3 & p2 = v._position;

	const glm::vec3 & n1 = _normal;
	const glm::vec3 & n2 = v._normal;

	const glm::vec2 & uv1 = _uv;
	const glm::vec2 & uv2 = v._uv;

	if (p1[0]>p2[0]+EPSILON) return false;
	if (p1[0]<p2[0]-EPSILON) return true;

	if (p1[1]>p2[1]+EPSILON) return false;
	if (p1[1]<p2[1]-EPSILON) return true;

	if (p1[2]>p2[2]+EPSILON) return false;
	if (p1[2]<p2[2]-EPSILON) return true;

	if (n1[0]>n2[0]+EPSILON) return false;
	if (n1[0]<n2[0]-EPSILON) return true;

	if (n1[1]>n2[1]+EPSILON) return false;
	if (n1[1]<n2[1]-EPSILON) return true;

	if (n1[2]>n2[2]+EPSILON) return false;
	if (n1[2]<n2[2]-EPSILON) return true;

	if (uv1[0]>uv2[0]+EPSILON) return false;
	if (uv1[0]<uv2[0]-EPSILON) return true;

	if (uv1[1]>uv2[1]+EPSILON) return false;
	if (uv1[1]<uv2[1]-EPSILON) return true;

	return false;
}
