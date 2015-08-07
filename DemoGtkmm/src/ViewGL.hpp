// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEWGL_HPP_
#define VIEWGL_HPP_

#include "Scene.hpp"

#include <Herve/Herve.hpp>

#include <gtkmm.h>
#include <gtkglmm.h>

class Controller;
class View;

// Displays an OpenGL scene in a gtkmm window.
class ViewGL : public Gtk::GL::DrawingArea {

	// MVC references
	Controller & _refController;
	View & _refView;

	// display info
	Herve::FpsCounter _fpsCounter;

	// keyboard
	Herve::InputMotion1dRelative _keyUp;
	Herve::InputMotion1dRelative _keyDown;
	Herve::InputMotion1dRelative _keyLeft;
	Herve::InputMotion1dRelative _keyRight;
	Herve::InputMotion1dRelative _keyForward;
	Herve::InputMotion1dRelative _keyBackward;

	// mouse
	Herve::InputMotion2d _mouse;

public:
	// todo multiGL
	//Herve::DisplayDeviceMono _displayDevice;
	Herve::DisplayDeviceStereoFake _displayDevice;

	Glib::RefPtr< Gdk::GL::Config > _glconfig;
	Glib::RefPtr< Gdk::GL::Context > _glcontext;


public:
	ViewGL(Controller & refController, View & refView, int & argc, char** argv);
	void init();

private:
	// override signal handlers
	bool on_expose_event(GdkEventExpose * event);
	bool on_configure_event(GdkEventConfigure * event);
	bool on_button_press_event(GdkEventButton* event);
	bool on_button_release_event(GdkEventButton* event);
	bool on_motion_notify_event(GdkEventMotion * event);

	// new handlers
	bool handle_idle();
	bool handle_key_press_event(GdkEventKey * event);
	bool handle_key_release_event(GdkEventKey * event);
};

// Duplicates an OpenGL view (same GPU context).
class ViewGLDup : public Gtk::GL::DrawingArea {

	Controller & _refController;
	ViewGL & _refViewGL;

public:
	ViewGLDup(Controller & refController, ViewGL & refViewGL);

private:
	bool on_expose_event(GdkEventExpose * event);
	bool on_configure_event(GdkEventConfigure * event);
	bool handle_idle();
};

#endif
