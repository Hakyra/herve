// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "View.hpp"

#include <Herve/Herve.hpp>

// Controller class in MVC pattern
class Controller {

	// view
	View _view;

	// model
	Herve::MapUptr<Herve::Scene> _scenes;
	Herve::Scene * _ptrCurrentScene;

public:
	Controller(int & argc, char ** argv);
	void run();
	Herve::Scene * getPtrCurrentScene();
	void selectScene(const std::string & text);
	std::vector<std::string> getSceneNames() const;
};

#endif
