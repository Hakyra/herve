// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Util.hpp>
void Herve::utilError(const char * message, const char * file, int line) {
	std::cout << "[error] " << message << " (" << file << ", " << line << ")\n";
	exit(-1);
}

void Herve::utilError(const char * message1, const char * message2, const char * file, int line) {
	std::cout << "[error] " << message1 << message2 << " (" << file << ", " << line << ")\n";
	exit(-1);
}

void Herve::utilInfo(const char * message, const char * file, int line) {
	std::cout << "[info] " << message << " (" << file << ", " << line << ")\n";
}

void Herve::utilInfo(const char * message1, const char * message2, const char * file, int line) {
	std::cout << "[info] " << message1 << message2 << " (" << file << ", " << line << ")\n";
}
