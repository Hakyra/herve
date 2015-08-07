// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/glm.hpp>

glm::vec3 operator*(double k, const glm::vec3 & v) {
	  return glm::vec3(k*v[0], k*v[1], k*v[2]);
}
