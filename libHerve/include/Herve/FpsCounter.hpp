// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_FPSCOUNTER_HPP_
#define HERVE_FPSCOUNTER_HPP_

#include <chrono>

namespace Herve {

/// \brief count FPS (frames per second)
///
class FpsCounter {
	std::chrono::time_point<std::chrono::system_clock> _t0;
	int _nbFrames;

public:
	/// \brief start counting fps
	///
	void start();

	/// \brief new frame rendered
	///
	void countNewFrame();

	/// \brief check if new fps value available (one second at least between two fps values)
	///
	bool hasFps() const;

	/// \brief get fps value and start counting new fps value
	///
	float getFpsAndRestart();
};

} // namespace Herve

#endif
