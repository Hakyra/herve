// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Random.hpp>

using namespace Herve;

Random::Random(int seed) :
		_distribution(0, 1) {
	_engine.seed(seed);
}

float Random::generate() {
	return _distribution(_engine);
}

Random randomInitFunc(const unsigned int idThread) {
	return Random(time(0) + idThread);
}
