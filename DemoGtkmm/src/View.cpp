// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "View.hpp"

View::View(Controller & refController, int & argc, char** argv) :
Gtk::Main(argc, argv),
Gtk::Window(),
_refController(refController),
_viewPanel(refController, *this),
_viewGL(refController, *this, argc, argv) {

	// main window
	set_title("DemoGtkmm");
	set_size_request(1000, 480);
	move(0, 0);

	// ovr window
	_window2.set_decorated(false);
	_window2.set_size_request(800, 500);
	_window2.move(1000, 0);
}

void View::run() {
	Gtk::Main::run(*this);
}

void View::updateFps(float fps) {
	_viewPanel.updateFps(fps);
}

void View::init() {

	// ovr window
	_window2.add(_viewGL);
	_window2.show_all();
	_viewGL.init();
	_viewGLDup.reset(new ViewGLDup(_refController, _viewGL));

	// main window
	add(_box);
	_box.pack_start(_viewPanel, Gtk::PACK_SHRINK);
	_box.pack_start(*_viewGLDup);
	show_all();
	// _viewPanel must be init after _viewGL (because of combo box init)
	_viewPanel.init();
}
