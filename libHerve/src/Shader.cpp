// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Shader.hpp>
#include <Herve/Util.hpp>

#include <sstream>
#include <fstream>

using namespace Herve;

void Shader::initFromFile(const std::string & vsFilename, const std::string & fsFilename) {

	std::string vsSource = readTextFile(vsFilename);
	std::string fsSource = readTextFile(fsFilename);

	if (not initFromString(vsSource, fsSource)) {
		std::ostringstream oss;
		oss << "Error in shader program (" << vsFilename << ", " << fsFilename << ")";
		utilInfo(oss.str().c_str(), __FILE__, __LINE__);
	}
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::bind() const {
	glUseProgram(_programId);
}

bool Shader::initFromString(const std::string & vertexShaderSource, const std::string & fragmentShaderSource) {

	_programId = glCreateProgram();

	GLuint vertexShader = loadShaderFromString(GL_VERTEX_SHADER, vertexShaderSource);
	if (vertexShader) glAttachShader(_programId, vertexShader);
	else {
		utilInfo("Error in vertex shader", __FILE__, __LINE__);
		return false;
	}

	GLuint fragmentShader = loadShaderFromString(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (fragmentShader) glAttachShader(_programId, fragmentShader);
	else {
		utilInfo("Error in fragment shader", __FILE__, __LINE__);
		return false;
	}

	glLinkProgram(_programId);

	unbind();

	return true;
}

GLuint Shader::loadShaderFromString(GLenum type, const std::string & source) const {

	// compile shader
	GLuint shader = glCreateShader(type);
	const char * ptrSource = source.c_str();
	glShaderSource(shader, 1, &ptrSource, NULL);
	glCompileShader(shader);

	// compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (GL_FALSE == status) {
		const int BUFFER_SIZE = 1e3;
		int size;
		char buffer[BUFFER_SIZE];
		glGetShaderInfoLog(shader,  BUFFER_SIZE,  &size,  buffer);
		utilInfo("Failed to compile shader", __FILE__, __LINE__);
		utilInfo(buffer, __FILE__, __LINE__);
		return 0;
	}

	return shader;
}

void Shader::uniform1i(const char * location, GLint i1) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform1i(id, i1);
}

void Shader::uniform1f(const char * location, GLfloat v1) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform1f(id,v1);
}

void Shader::uniform2i(const char * location, GLint i1, GLint i2) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform2i(id, i1, i2);	
}

void Shader::uniform2f(const char * location, GLfloat v1, GLfloat v2) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform2f(id,v1, v2);
}

void Shader::uniform3f(const char * location, GLfloat v1, GLfloat v2, GLfloat v3) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform3f(id,v1, v2, v3);
}

void Shader::uniform4f(const char * location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniform4f(id,v1, v2, v3, v4);
}

void Shader::uniformMatrix4fv(const char * location, const GLfloat* value) const {
	GLuint id = glGetUniformLocation(_programId, location);
	glUniformMatrix4fv(id, 1, GL_FALSE, value);
}

std::string Shader::readTextFile(const std::string & filename) const {

	std::ifstream file(filename);
	std::string sourceCode;

	if(file) {
		std::string line;
		while(getline(file, line))
			sourceCode += "\n" + line;
	}
	else {
		utilInfo("Failed to read shader file : ", filename.c_str(), __FILE__, __LINE__);
	}

	return sourceCode;
}
