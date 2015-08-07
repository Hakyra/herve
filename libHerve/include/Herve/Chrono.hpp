// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_CHRONOMETRE_HPP_
#define HERVE_CHRONOMETRE_HPP_

#include <chrono>

namespace Herve {

/// \brief Chrono.
///
class Chrono {
	typedef std::chrono::time_point<std::chrono::system_clock> Time;
	Time _t0;
	Time _t1;

public:
	/// \brief start chrono
	///
	void start();

	/// \brief stop chrono
	///
	void stop();

	/// \brief elapsed time between start and stop
	///
	float elapsedStopped() const;

	/// \brief elapsed time between start and now
	///
	float elapsedRunning();
};

} // namespace Herve

#endif
