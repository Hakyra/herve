// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_UTIL_HPP_
#define HERVE_UTIL_HPP_

#include <iostream>
#include <map>
#include <memory>
#include <vector>

namespace Herve {

template<typename T> using VectorUptr = std::vector< std::unique_ptr<T> >;
template<typename T> using MapUptr = std::map< std::string, std::unique_ptr<T> >;

/// \brief display a message and exit program
///
/// usage: Herve::utilError("my message", __FILE__, __LINE__);
void utilError(const char * message, const char * file, int line);

/// \brief display a message and exit program
///
/// usage: Herve::utilError("my message 1", "my message 2", __FILE__, __LINE__);
void utilError(const char * message1, const char * message2, const char * file, int line);

/// \brief display a message
///
/// usage: Herve::utilInfo("my message", __FILE__, __LINE__);
void utilInfo(const char * message, const char * file, int line);

/// \brief display a message
///
/// usage: Herve::utilInfo("my message 1", "my message 2", __FILE__, __LINE__);
void utilInfo(const char * message1, const char * message2, const char * file, int line);

} // namespace Herve

#endif
