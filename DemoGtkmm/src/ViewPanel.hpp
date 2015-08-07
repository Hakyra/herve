// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEWPANEL_HPP_
#define VIEWPANEL_HPP_

#include <gtkmm.h>

class Controller;
class View;

// Display a panel in main window (choose scene, display FPS).
class ViewPanel : public Gtk::VBox {

	Controller & _refController;
	View & _refView;

	Gtk::Label _sceneLabel;
	Gtk::ComboBoxText _sceneComboBox;

	Gtk::Label _docLabel;

	Gtk::Label _fpsLabel;

public:
	ViewPanel(Controller & refController, View & refView);
	void init();

	// Update FPS display in the panel.
	void updateFps(float fps);

private:
	void handle_scene_changed();
};

#endif
