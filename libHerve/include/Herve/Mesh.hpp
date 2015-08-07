// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_MESH_HPP_
#define HERVE_MESH_HPP_

#include <Herve/glm.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <vector>

namespace Herve {

/// \brief Mesh (VAO) = set of vertex buffers (VBO)
///
class Mesh {

	class VertexBuffer {
		GLuint _vertexBufferId;
		GLuint _index;
		GLint _nbValuesPerVertex;

	public:
		VertexBuffer(GLuint index, GLsizei nbVertices, GLint nbValuesPerVertex, GLfloat * data);
		void enable() const;
		void disable() const;
	};

protected:
	GLenum _meshMode;
	GLuint _vertexArrayId;
	std::vector<VertexBuffer> _vertexBuffers;
	GLsizei _nbVertices;

public:
	/// \brief initialize a mesh of the given mode (triangles, triangle strip...)
	///
	/// after init, pushVertexBuffer can be called
	void initMesh(GLenum meshMode, GLsizei nbVertices);

	/// \brief add a buffer for this vertex array (number buffers with 0, 1 ...)
	///
	/// call init (once) before pushing vertex buffers
	void pushVertexBuffer(GLint nbValuesPerVertex, GLfloat * data);

	/// \brief draw the vertex array object
	///
	virtual void draw() const;
};

/// \brief Mesh with indexed vertices
///
class ElementMesh : public Mesh {

	GLuint _elementBufferId;
	GLsizei _nbElements;

public :
	/// \brief load index buffer
	///
	void initElementBuffer(GLsizei nbElements, GLuint * data);

	/// \brief draw the vertex array object
	///
	virtual void draw() const;
};

} // namespace Herve

#endif
