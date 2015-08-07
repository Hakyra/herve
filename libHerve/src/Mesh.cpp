// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Mesh.hpp>

using namespace Herve;

//////////////////////////////////////////////////////////////////////
// Mesh::VertexBuffer
//////////////////////////////////////////////////////////////////////

Mesh::VertexBuffer::VertexBuffer(GLuint index, GLsizei nbVertices, GLint nbValuesPerVertex, GLfloat * data) :
_vertexBufferId(0), _index(index), _nbValuesPerVertex(nbValuesPerVertex) {

	glGenBuffers(1, &_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, nbVertices*nbValuesPerVertex*sizeof(GL_FLOAT), data, GL_STATIC_DRAW);
}

void Mesh::VertexBuffer::enable() const {
	glEnableVertexAttribArray(_index);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glVertexAttribPointer(_index, _nbValuesPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::VertexBuffer::disable() const {
	glDisableVertexAttribArray(_index);
}

//////////////////////////////////////////////////////////////////////
// Mesh
//////////////////////////////////////////////////////////////////////

void Mesh::initMesh(GLenum meshMode, GLsizei nbVertices) {

	_meshMode = meshMode;
	_nbVertices = nbVertices;

	glGenVertexArrays(1, &_vertexArrayId);
}


void Mesh::pushVertexBuffer(GLint nbValuesPerVertex, GLfloat * data) {

	glBindVertexArray(_vertexArrayId);

	GLuint index = _vertexBuffers.size();

	_vertexBuffers.push_back(VertexBuffer(index, _nbVertices, nbValuesPerVertex, data));

	glBindVertexArray(0);
}

void Mesh::draw() const {

	// bind VAO
	glBindVertexArray(_vertexArrayId);

	// bind VBO
	for (auto& b : _vertexBuffers) b.enable();

	// draw array
	glDrawArrays(_meshMode, 0, _nbVertices);

	// unbind VBO
	for (auto& b : _vertexBuffers) b.disable();

	// unbind VAO
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////
// ElementMesh
//////////////////////////////////////////////////////////////////////

void ElementMesh::initElementBuffer(GLsizei nbElements, GLuint* data) {

	_nbElements = nbElements;

	glBindVertexArray(_vertexArrayId);

	glGenBuffers(1, &_elementBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nbElements * sizeof(GLuint), &data[0] , GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ElementMesh::draw() const {

	// bind VAO
	glBindVertexArray(_vertexArrayId);

	// bind VBO
	for (auto& b : _vertexBuffers) b.enable();

	// bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);

	// draw elements
	glDrawElements(_meshMode, _nbElements, GL_UNSIGNED_INT, 0);

	// unbind VBO
	for (auto& b : _vertexBuffers) b.disable();

	// unbind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind VAO
	glBindVertexArray(0);
}
