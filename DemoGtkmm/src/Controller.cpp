// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Controller.hpp"
#include "Scene.hpp"
#include "SceneOvr.hpp"

Controller::Controller(int & argc, char ** argv) : _view(*this, argc, argv) {

	// add scenes
	_scenes.insert(Herve::MapUptr<Herve::Scene>::value_type(std::string("points"), std::unique_ptr<Herve::Scene>(new ScenePoints)));
	_scenes.insert(Herve::MapUptr<Herve::Scene>::value_type(std::string("sibenik"), std::unique_ptr<Herve::Scene>(new Scene("sibenik_lux.obj"))));
}

void Controller::run() {
	  _view.init();
	  _view.run();
}

Herve::Scene * Controller::getPtrCurrentScene() {
	return _ptrCurrentScene;
}

void Controller::selectScene(const std::string& text) {
	Herve::MapUptr<Herve::Scene>::const_iterator iter = _scenes.find(text);
	if (iter != _scenes.end()) _ptrCurrentScene = iter->second.get();
}

std::vector<std::string> Controller::getSceneNames() const {
	std::vector<std::string> names;
	for (auto & p : _scenes) {
		names.push_back(p.first);
	}
	return names;
}
