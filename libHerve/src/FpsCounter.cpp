// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/FpsCounter.hpp>

using namespace Herve;

void FpsCounter::start() {
	_t0 = std::chrono::system_clock::now();
	_nbFrames = 0;
}

void FpsCounter::countNewFrame() {
	_nbFrames++;
}

bool FpsCounter::hasFps() const {
	if (_nbFrames > 0) {
		std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
		float deltaT = 1e-3 * std::chrono::duration_cast<std::chrono::milliseconds>(t1 - _t0).count();
		if (deltaT >= 1.f) {
			return true;
		}
	}
	return false;
}

float FpsCounter::getFpsAndRestart() {
	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
	float deltaT = 1e-3 * std::chrono::duration_cast<std::chrono::milliseconds>(t1 - _t0).count();
	float fps = _nbFrames / deltaT;
	start();
	return fps;
}
