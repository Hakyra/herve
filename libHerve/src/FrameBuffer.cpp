// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/FrameBuffer.hpp>
#include <Herve/Util.hpp>

#include <cassert>

using namespace Herve;

void FrameBuffer::init(int width, int height) {
	assert(_isInitOk == false);

	// init members
	_width = width;
	_height = height;

	// create frame buffer
	glGenFramebuffers(1, &_frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);

	// create and init texture
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, _width, _height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create depth buffer
	glGenRenderbuffers(1, &_renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBufferId);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureId, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	// check whether everything is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		utilError("Failed to create FB", __FILE__, __LINE__);
	}

	unbindTexture();
	unbindFrameBuffer();
	_isInitOk = true;
}

void FrameBuffer::bindFrameBuffer() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
}

void FrameBuffer::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture() const {
	glBindTexture(GL_TEXTURE_2D, _textureId);
}

FrameBuffer::FrameBuffer() : _isInitOk(false) {
}
void FrameBuffer::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int FrameBuffer::getWidth() const {
	return _width;
}

int FrameBuffer::getHeight() const {
	return _height;
}

void FrameBuffer::clear(float r, float g, float b, float a) {
	bindFrameBuffer();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _width, _height);
	unbindFrameBuffer();
}

/*
FrameBuffer::~FrameBuffer() {
	if (_isInitOk) {
		glDeleteFramebuffers(1, &_frameBufferId);
		glDeleteTextures(1, &_textureId);
		glDeleteRenderbuffers(1, &_renderBufferId);
	}
}
*/

