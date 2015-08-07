// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "ViewGL.hpp"

#include "Controller.hpp"
#include "View.hpp"

ViewGL::ViewGL(Controller & refController, View & refView, int& argc, char** argv) :
_refController(refController),
_refView(refView),
_keyUp(5),
_keyDown(5),
_keyLeft(5),
_keyRight(5),
_keyForward(5),
_keyBackward(5),
_mouse(0, 0, 0.002, 0.002)
,_displayDevice(1024, 1024)
{

	// initialize gtkglextmm
	Gtk::GL::init(argc, argv);
	_glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	if (not _glconfig)
		Herve::utilError("cannot initialise OpenGL", __FILE__, __LINE__);
	set_gl_capability(_glconfig);

	// initialize event handling
	//Glib::signal_idle().connect(sigc::mem_fun(*this, &ViewGL::handle_idle));
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &ViewGL::handle_idle), 20);
	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::BUTTON1_MOTION_MASK);
	add_events(Gdk::KEY_PRESS_MASK);
	add_events(Gdk::KEY_RELEASE_MASK);
	signal_key_press_event().connect(sigc::mem_fun(*this, &ViewGL::handle_key_press_event));
	signal_key_release_event().connect(sigc::mem_fun(*this, &ViewGL::handle_key_release_event));
	set_can_focus(true);
}

void ViewGL::init() {

	_glcontext = get_gl_context();

	// begin OpenGL
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	if (!glwindow->gl_begin(get_gl_context()))
		return;

	// initialize device
	_displayDevice.initDevice();
	_displayDevice.initDisplay(0, 0);

	// initialize scenes
	std::vector<std::string> sceneNames = _refController.getSceneNames();
	for (const std::string & name : sceneNames) {
		_refController.selectScene(name);
		_refController.getPtrCurrentScene()->init();
	}

	// end OpenGL
	glwindow->gl_end();

	// start counting fps
	_fpsCounter.start();
}

bool ViewGL::on_expose_event(GdkEventExpose* event) {

	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	if (not glwindow->gl_begin(_glcontext))
		return false;

	// update camera
	float tx = _keyRight.getValue() - _keyLeft.getValue();
	float ty = _keyUp.getValue() - _keyDown.getValue();
	float tz = _keyBackward.getValue() - _keyForward.getValue();
	float rx = -_mouse.getYValue();	// mouse y controls rotation around opengl x-axis
	float ry = -_mouse.getXValue();	// mouse x controls rotation around opengl y-axis
	_displayDevice.updateView(tx, ty, tz, rx, ry, 0.f);

	// render scene
	Herve::Scene * ptrScene = _refController.getPtrCurrentScene();
	if (ptrScene) {
		_displayDevice.render(ptrScene);
		glwindow->swap_buffers();
	}
	glwindow->gl_end();

	return true;
}

bool ViewGL::on_configure_event(GdkEventConfigure * event) {
	_displayDevice.resizeWindow(event->width, event->height);
	return true;
}

bool ViewGL::on_button_press_event(GdkEventButton* event) {
	if (event->button == 1)
		_mouse.press(event->x, event->y);
	return true;
}

bool ViewGL::on_button_release_event(GdkEventButton* event) {
	if (event->button == 1)
		_mouse.release(event->x, event->y);
		return true;
}

bool ViewGL::on_motion_notify_event(GdkEventMotion* event) {
	_mouse.motion(event->x, event->y);
	return true;
}

bool ViewGL::handle_idle() {

	// FIXME beurk
	grab_focus();

	// update fps
	_fpsCounter.countNewFrame();
	if (_fpsCounter.hasFps())
		_refView.updateFps(_fpsCounter.getFpsAndRestart());

	// refresh opengl view
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win) win->invalidate(false);

	return true;
}

bool ViewGL::handle_key_press_event(GdkEventKey * event) {

	switch (event->keyval) {
	case GDK_KEY_Page_Up : _keyUp.press(); break;
	case GDK_KEY_Page_Down : _keyDown.press(); break;
	case GDK_KEY_Up : _keyForward.press(); break;
	case GDK_KEY_Down : _keyBackward.press(); break;
	case GDK_KEY_Left : _keyLeft.press(); break;
	case GDK_KEY_Right : _keyRight.press(); break;
	}

	return true;
}

bool ViewGL::handle_key_release_event(GdkEventKey * event) {

	switch (event->keyval) {
	case GDK_KEY_Page_Up : _keyUp.release(); break;
	case GDK_KEY_Page_Down : _keyDown.release(); break;
	case GDK_KEY_Up : _keyForward.release(); break;
	case GDK_KEY_Down : _keyBackward.release(); break;
	case GDK_KEY_Left : _keyLeft.release(); break;
	case GDK_KEY_Right : _keyRight.release(); break;
	case GDK_KEY_Escape : Gtk::Main::quit();
	}

	return true;
}

ViewGLDup::ViewGLDup(Controller & refController, ViewGL& refViewGL) :
Gtk::GL::DrawingArea(refViewGL._glconfig, refViewGL._glcontext, true, Gdk::GL::RGBA_TYPE),
_refController(refController),
_refViewGL(refViewGL) {

	// refresh the dup view every 500 ms
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &ViewGLDup::handle_idle), 500);
}

bool ViewGLDup::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	if (not glwindow->gl_begin(get_gl_context()))
		return false;

	glEnable(GL_DEPTH_TEST);

	// render scene
	Herve::Scene * ptrScene = _refController.getPtrCurrentScene();
	if (ptrScene) {
		_refViewGL._displayDevice.render(ptrScene);
		glwindow->swap_buffers();
	}
	glwindow->gl_end();

	return true;
}

bool ViewGLDup::on_configure_event(GdkEventConfigure* event) {
	_refViewGL._displayDevice.resizeWindow(event->width, event->height);
	return true;
}

bool ViewGLDup::handle_idle() {
	// refresh opengl view
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win) win->invalidate(false);

	return true;
}
