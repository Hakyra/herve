// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_FRAMEBUFFER_HPP_
#define HERVE_FRAMEBUFFER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>

namespace Herve {

/// \brief framebuffer object for render-to-texture (GPU)
///
class FrameBuffer {
	int _width;
	int _height;

	GLuint _frameBufferId;
	GLuint _textureId;
	GLuint _renderBufferId;

	bool _isInitOk;

public:
	/// \brief use this constructor
	///
	FrameBuffer();

	/// \brief create FBO (in GPU memory)
	///
	void init(int width, int height);

	/// \brief clear FBO
	///
	void clear(float r, float g, float b, float a);

	/// \brief bind FBO (begin FBO rendering)
	///
	void bindFrameBuffer() const;

	/// \brief unbind FBO (end FBO rendering)
	///
	static void unbindFrameBuffer();

	/// \brief bind FBO (as a texture)
	///
	void bindTexture() const;

	/// \brief unbind FBO (as a texture)
	///
	static void unbindTexture();

	/// \brief FBO width
	///
	int getWidth() const;

	/// \brief FBO height
	///
	int getHeight() const;
};

} // namespace Herve

#endif
