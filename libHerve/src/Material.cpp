// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Material.hpp>

using namespace Herve;

Material::Material() :
	_ka(0.f), _kd(0.5f), _ks(1.f), _ns(10), _ptrTexture(nullptr) {
}
