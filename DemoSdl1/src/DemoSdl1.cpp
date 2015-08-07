// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

#include <Herve/Herve.hpp>
#include <HerveOvr/HerveOvr.hpp>

#include <SDL.h>
#include <memory>

const int JOYSTICK_THRESHOLD = 5000;
bool gEnableJoystick = true;

// window data
SDL_Surface * gPtrSurface;
bool gFullscreen = false;
int oldWidth, oldHeight;

// translations (keyboard)
Herve::InputMotion1dRelative gKeyForward(6);
Herve::InputMotion1dRelative gKeyBackward(6);
Herve::InputMotion1dRelative gKeyLeft(6);
Herve::InputMotion1dRelative gKeyRight(6);
Herve::InputMotion1dRelative gKeyUp(6);
Herve::InputMotion1dRelative gKeyDown(6);

// joystick
Herve::InputMotion1dRelative gJoyGoForward(4);
Herve::InputMotion1dRelative gJoyGoBackward(4);
Herve::InputMotion1dRelative gJoyGoLeft(4);
Herve::InputMotion1dRelative gJoyGoRight(4);
Herve::InputMotion1dRelative gJoyGoUp(4);
Herve::InputMotion1dRelative gJoyGoDown(4);
Herve::InputMotion1dAbsolute gJoyLookLeft(1);
Herve::InputMotion1dAbsolute gJoyLookRight(1);
Herve::InputMotion1dAbsolute gJoyLookUp(0.5);
Herve::InputMotion1dAbsolute gJoyLookDown(0.5);

// orientation (mouse)
Herve::InputMotion2d gMouse(0, 0, 0.002, 0.002);

// scene data
std::unique_ptr<Herve::DisplayDevice> gUptrDisplayDevice;
Herve::FpsCounter gFpsCounter;
Scene gScene;

void switchFullscreen() {
	gFullscreen = not gFullscreen;
	if (gFullscreen) {
		oldWidth = gUptrDisplayDevice->getWidth();
		oldHeight = gUptrDisplayDevice->getHeight();
		gUptrDisplayDevice->resizeWindow(gUptrDisplayDevice->getMaxWidth(), gUptrDisplayDevice->getMaxHeight());
		gPtrSurface = SDL_SetVideoMode(gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL );
		SDL_WM_ToggleFullScreen(gPtrSurface);
	}
	else {
		gUptrDisplayDevice->resizeWindow(oldWidth, oldHeight);
		gPtrSurface = SDL_SetVideoMode(gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL|SDL_RESIZABLE );
	}
}

void render() {
	// print FPS
	gFpsCounter.countNewFrame();
	if (gFpsCounter.hasFps()) {
		std::cout << "\rFPS = " << gFpsCounter.getFpsAndRestart() << "\t";
		std::cout.flush();
	}

	// render the scene
	gUptrDisplayDevice->render(&gScene);
	SDL_GL_SwapBuffers();
}

void resize(const SDL_ResizeEvent & event) {
	int w = event.w;
	int h = std::max(1, event.h);
	gUptrDisplayDevice->resizeWindow(w, h);
	gPtrSurface = SDL_SetVideoMode(gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL|SDL_RESIZABLE );
}

// joystick
void joyMotion(const SDL_JoyAxisEvent & event) {

	if (not gEnableJoystick) return;

	// go forward/backward
	if( event.axis == 1 ) {
		// if up
		if (event.value < -JOYSTICK_THRESHOLD) {
			gJoyGoBackward.release();
			gJoyGoForward.press();
		}
		// if down
		else if (event.value > JOYSTICK_THRESHOLD){
			gJoyGoBackward.press();
			gJoyGoForward.release();
		}
		// if neutral
		else {
			gJoyGoBackward.release();
			gJoyGoForward.release();
		}
	}

	// go left/right
	if( event.axis == 0 ) {
		if (event.value < -JOYSTICK_THRESHOLD) {
			gJoyGoRight.release();
			gJoyGoLeft.press();
		}
		// if right
		else if (event.value > JOYSTICK_THRESHOLD){
			gJoyGoRight.press();
			gJoyGoLeft.release();
		}
		// if neutral
		else {
			gJoyGoRight.release();
			gJoyGoLeft.release();
		}
	}

	// go up
	if( event.axis == 5 ) {
		if (event.value > 0) gJoyGoUp.press();
		else gJoyGoUp.release();
	}

	// go down
	if( event.axis == 2 ) {
		if (event.value > 0) gJoyGoDown.press();
		else gJoyGoDown.release();
	}

	// look up/down
	if( event.axis == 4 ) {
		// if up
		if (event.value < -JOYSTICK_THRESHOLD) {
			gJoyLookDown.release();
			gJoyLookUp.press();
		}
		// if down
		else if (event.value > JOYSTICK_THRESHOLD){
			gJoyLookDown.press();
			gJoyLookUp.release();
		}
		// if neutral
		else {
			gJoyLookDown.release();
			gJoyLookUp.release();
		}
	}

	// look left/right
	if( event.axis == 3 ) {
		// if left
		if (event.value < -JOYSTICK_THRESHOLD) {
			gJoyLookRight.press();
			gJoyLookLeft.release();
		}
		// if right
		else if (event.value > JOYSTICK_THRESHOLD){
			gJoyLookRight.release();
			gJoyLookLeft.press();
		}
		// if neutral
		else {
			gJoyLookRight.release();
			gJoyLookLeft.release();
		}
	}
}

void updateCamera() {

	float tx = gKeyRight.getValue() - gKeyLeft.getValue()
    		+ gJoyGoRight.getValue() - gJoyGoLeft.getValue();
	float ty = gKeyUp.getValue() - gKeyDown.getValue()
    		+ gJoyGoUp.getValue() - gJoyGoDown.getValue();
	float tz = gKeyBackward.getValue() - gKeyForward.getValue()
    		+ gJoyGoBackward.getValue() - gJoyGoForward.getValue();
	float rx = -gMouse.getYValue()	// mouse y controls rotation around opengl x-axis
    		+ gJoyLookUp.getValue() - gJoyLookDown.getValue();
	float ry = -gMouse.getXValue()	// mouse x controls rotation around opengl y-axis
    		+ gJoyLookRight.getValue() - gJoyLookLeft.getValue();
	gUptrDisplayDevice->updateView(tx, ty, tz, rx, ry, 0.f);
}

void mouseMotion(const SDL_MouseMotionEvent & event) {
	gMouse.motion(event.x, event.y);
}

void mouseDown(const SDL_MouseButtonEvent & event) {
	if (event.button == SDL_BUTTON_LEFT) {
		gMouse.press(event.x, event.y);
	}
}

void mouseUp(const SDL_MouseButtonEvent & event) {
	if (event.button == SDL_BUTTON_LEFT) {
		gMouse.release(event.x, event.y);
	}
}

void keyDown(const SDL_KeyboardEvent & event) {
	switch (event.keysym.sym) {
	case SDLK_UP :       gKeyForward.press(); break;
	case SDLK_DOWN :     gKeyBackward.press(); break;
	case SDLK_PAGEDOWN : gKeyDown.press(); break;
	case SDLK_PAGEUP :   gKeyUp.press(); break;
	case SDLK_LEFT :     gKeyLeft.press(); break;
	case SDLK_RIGHT :    gKeyRight.press(); break;
	case SDLK_ESCAPE :
		std::cout << std::endl;
		exit(0);
		break;
	default: break;
	}
}

void keyUp(const SDL_KeyboardEvent & event) {
	switch (event.keysym.sym) {
	case SDLK_UP :       gKeyForward.release(); break;
	case SDLK_DOWN :     gKeyBackward.release(); break;
	case SDLK_PAGEDOWN : gKeyDown.release(); break;
	case SDLK_PAGEUP :   gKeyUp.release(); break;
	case SDLK_LEFT :     gKeyLeft.release(); break;
	case SDLK_RIGHT :    gKeyRight.release(); break;
	case SDLK_f : switchFullscreen(); break;
	default: break;
	}
}

int main(int argc, char **argv) {

	//////////////////////////////////////////////////////////////////////
	// check joystick
	//////////////////////////////////////////////////////////////////////

	std::string commandNoJoystick = std::string(argv[0]) +  " --no-joystick ";
	Herve::utilInfo("to disable joystick, use : ", commandNoJoystick.c_str() , __FILE__, __LINE__);
	if (argc == 2 and std::string(argv[1]) == "--no-joystick")
		gEnableJoystick = false;

	//////////////////////////////////////////////////////////////////////
	// init
	//////////////////////////////////////////////////////////////////////

	// display device
	gUptrDisplayDevice.reset(new HerveOvr::DisplayDeviceStereoOvr(1024, 1024));
	if (not gUptrDisplayDevice->initDevice()) {
		gUptrDisplayDevice.reset(new Herve::DisplayDeviceMono());
		//gUptrDisplayDevice.reset(new Herve::DisplayDeviceStereoFake(1024, 1024));
		gUptrDisplayDevice->initDevice();
	}

	// SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
		Herve::utilError("SDL_Init", "", __FILE__, __LINE__);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int maxWidth = info->current_w;
	int maxHeight = info->current_h;
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	gPtrSurface = SDL_SetVideoMode( gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL|SDL_RESIZABLE );
	if( gPtrSurface == NULL )
		Herve::utilError("SDL_SetVideoMode", "", __FILE__, __LINE__);
	atexit(SDL_Quit);
	SDL_EnableUNICODE( SDL_TRUE );
	SDL_WM_SetCaption( "DemoSdl1", NULL );

	// joysticks
	int nbJoysticks = SDL_NumJoysticks();
	for(int i=0; i<nbJoysticks; i++) {
		SDL_Joystick * stick = SDL_JoystickOpen( i );
		if( stick ) {
			Herve::utilInfo("joystick = ", SDL_JoystickName(i), __FILE__, __LINE__);
		}
	}

	// scene data
	gUptrDisplayDevice->initDisplay(maxWidth, maxHeight);
	gScene.init(gUptrDisplayDevice.get());
	gFpsCounter.start();

	Herve::utilInfo("width = ", std::to_string(gUptrDisplayDevice->getWidth()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("height = ", std::to_string(gUptrDisplayDevice->getHeight()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("maxWidth = ", std::to_string(gUptrDisplayDevice->getMaxWidth()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("maxHeight = ", std::to_string(gUptrDisplayDevice->getMaxHeight()).c_str(), __FILE__, __LINE__);


	//////////////////////////////////////////////////////////////////////
	// main loop
	//////////////////////////////////////////////////////////////////////

	SDL_Event event;
	while (true)	{
		// handle events
		while( SDL_PollEvent( &event ) ) {
			switch(event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				keyDown(event.key);
				break;
			case SDL_KEYUP:
				keyUp(event.key);
				break;
			case SDL_MOUSEBUTTONUP :
				mouseUp(event.button);
				break;
			case SDL_MOUSEBUTTONDOWN :
				mouseDown(event.button);
				break;
			case SDL_MOUSEMOTION :
				mouseMotion(event.motion);
				break;
			case SDL_VIDEORESIZE :
				resize(event.resize);
				break;
			case SDL_JOYAXISMOTION :
				joyMotion(event.jaxis);
				break;
			}
		}

		updateCamera();
		render();
	}

	return 0;
}
