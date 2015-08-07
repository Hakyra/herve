// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_RANDOM_HPP_
#define HERVE_RANDOM_HPP_

#include <random>

namespace Herve {

/// \brief Random Number Generator using C++11 mt19937 (Mersenne-Twister)
///
class Random {

	std::mt19937_64 _engine;
	std::uniform_real_distribution<float> _distribution;

public:
	/// \brief Construct the RNG using a given seed
	///
	Random(int seed = 0);

	///\brief Generate a random number in [0, 1) using a uniform distribution.
	///
	float generate();
};

} // namespace Herve

#endif
