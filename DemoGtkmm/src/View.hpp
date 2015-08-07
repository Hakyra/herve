// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include "ViewPanel.hpp"
#include "ViewGL.hpp"

#include <gtkmm.h>

class Controller;

// View class in MVC pattern
class View : public Gtk::Main, public Gtk::Window {

	Controller & _refController;

	// main window
	Gtk::HBox _box;
	ViewPanel _viewPanel;
	std::unique_ptr<ViewGLDup> _viewGLDup;

	// ovr window
	Gtk::Window _window2;
	ViewGL _viewGL;

public :
	View (Controller & refController, int & argc, char ** argv);
	void init();
	void run();

	void updateFps(float fps);
};

#endif
