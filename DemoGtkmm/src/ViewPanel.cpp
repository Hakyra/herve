// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Controller.hpp"
#include "View.hpp"

#include "ViewPanel.hpp"

#include <sstream>
#include <iomanip>

ViewPanel::ViewPanel(Controller & refController, View & refView) :
_refController(refController),
_refView(refView),
_sceneLabel(" scene: "),
_docLabel("\n move with arrow/page keys \n\n change viewpoint with clic+drag \n"),
_fpsLabel("") {

	// scene
	_sceneLabel.set_alignment(Gtk::ALIGN_START);
	pack_start(_sceneLabel, Gtk::PACK_SHRINK);
	pack_start(_sceneComboBox, Gtk::PACK_SHRINK);
	_sceneComboBox.signal_changed().connect(sigc::mem_fun(*this, &ViewPanel::handle_scene_changed));

	// documentation
	_docLabel.set_alignment(Gtk::ALIGN_START);
	pack_start(_docLabel, Gtk::PACK_SHRINK);

	// fps
	_fpsLabel.set_alignment(Gtk::ALIGN_START);
	pack_start(_fpsLabel, Gtk::PACK_SHRINK);
}

void ViewPanel::updateFps(float fps) {
	std::ostringstream oss;
	oss << " FPS = " << std::setprecision(2) << std::fixed << fps;
	_fpsLabel.set_text(oss.str());
}

void ViewPanel::handle_scene_changed() {
	std::string text = _sceneComboBox.get_active_text();
	_refController.selectScene(text);
}

void ViewPanel::init() {
	// fill combo box with scene names
	std::vector<std::string> sceneNames = _refController.getSceneNames();
	for (const std::string & name : sceneNames)
		_sceneComboBox.append(name);

	// select the last scene in the combo box
	// this will send a signal_changed therefore select the scene in the controller
	_sceneComboBox.set_active_text(sceneNames.back());
}
