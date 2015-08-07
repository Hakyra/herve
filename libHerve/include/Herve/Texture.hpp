// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_TEXTURE_HPP_
#define HERVE_TEXTURE_HPP_

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

namespace Herve {

/// \brief 2D RGB or RGBA texture, from PNG file
///
class Texture {

	GLuint _textureId;

public:
	/// \brief create a texture in GPU with no data
	///
	void initEmpty();

	/// \brief create a texture in GPU from a PNG file
	///
	bool initFromFile(const std::string & filename);

	/// \brief bind texture
	///
	void bind() const;

	/// \brief unbind texture
	///
	static void unbind();
};

} // namespace Herve

#endif
