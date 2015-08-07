// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SHADER_HPP_
#define HERVE_SHADER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

namespace Herve {

/// \brief shader program (vertex shader + fragment shader)
///
class Shader {

	GLuint _programId;

public:

	/// \brief initialize a shader program from a string
	///
	bool initFromString(const std::string & vertexShaderSource, const std::string & fragmentShaderSource);

	/// \brief initialize a shader program from a file
	///
	void initFromFile(const std::string & vsFilename, const std::string & fsFilename);

	/// \brief bind shader program
	///
	void bind() const;

	/// \brief unbind shader program
	///
	static void unbind();

	/// \brief load parameter in shader program (int)
	///
	void uniform1i(const char * location, GLint i1) const;

	/// \brief load parameter in shader program (float)
	///
	void uniform1f(const char * location, GLfloat v1) const;

	/// \brief load parameter in shader program (ivec2)
	///
	void uniform2i(const char * location, GLint i1, GLint i2) const;

	/// \brief load parameter in shader program (vec2)
	///
	void uniform2f(const char * location, GLfloat v1, GLfloat v2) const;

	/// \brief load parameter in shader program (vec3)
	///
	void uniform3f(const char * location, GLfloat v1, GLfloat v2, GLfloat v3) const;

	/// \brief load parameter in shader program (vec4)
	///
	void uniform4f(const char * location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const;

	/// \brief load parameter in shader program (mat4)
	///
	void uniformMatrix4fv(const char * location,  const GLfloat *value) const;

private:
	GLuint loadShaderFromString(GLenum type, const std::string & source) const;
	std::string readTextFile(const std::string & filename) const;
};

} // namespace Herve

#endif
