// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

#include <Herve/Herve.hpp>
//#include <HerveOvr/HerveOvr.hpp>

#include <SDL.h>
#include <memory>

// window data
SDL_Surface * gPtrSurface;
bool gFullscreen = false;
int oldWidth, oldHeight;

// scene data
std::unique_ptr<Herve::DisplayDevice> gUptrDisplayDevice;
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
	gUptrDisplayDevice->render(&gScene);
	SDL_GL_SwapBuffers();
}

void resize(const SDL_ResizeEvent & event) {
	int w = event.w;
	int h = std::max(1, event.h);
	gUptrDisplayDevice->resizeWindow(w, h);
	gPtrSurface = SDL_SetVideoMode(gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL|SDL_RESIZABLE );
}

void keyUp(const SDL_KeyboardEvent & event) {
	switch (event.keysym.sym) {
	case SDLK_f : switchFullscreen(); break;
	case SDLK_ESCAPE : exit(0); break;
	default: break;
	}
}

int main(int argc, char **argv) {

	//////////////////////////////////////////////////////////////////////
	// init
	//////////////////////////////////////////////////////////////////////

	// display device
    /*
	gUptrDisplayDevice.reset(new HerveOvr::DisplayDeviceStereoOvr(1024, 1024));
	if (not gUptrDisplayDevice->initDevice()) {
		gUptrDisplayDevice.reset(new Herve::DisplayDeviceMono());
		gUptrDisplayDevice->initDevice();
	}
    */
    gUptrDisplayDevice.reset(new Herve::DisplayDeviceMono());
    gUptrDisplayDevice->initDevice();

	// SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) Herve::utilError("SDL_Init", "", __FILE__, __LINE__);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int maxWidth = info->current_w;
	int maxHeight = info->current_h;
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	gPtrSurface = SDL_SetVideoMode( gUptrDisplayDevice->getWidth(), gUptrDisplayDevice->getHeight(), 24, SDL_OPENGL|SDL_RESIZABLE );
	if( gPtrSurface == NULL ) Herve::utilError("SDL_SetVideoMode", "", __FILE__, __LINE__);
	atexit(SDL_Quit);
	SDL_EnableUNICODE( SDL_TRUE );
	SDL_WM_SetCaption( "Demo", NULL );

	// scene data
	gUptrDisplayDevice->initDisplay(maxWidth, maxHeight);
	gScene.init(gUptrDisplayDevice.get());

	// print screen sizes
	Herve::utilInfo("width = ", std::to_string(gUptrDisplayDevice->getWidth()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("height = ", std::to_string(gUptrDisplayDevice->getHeight()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("maxWidth = ", std::to_string(gUptrDisplayDevice->getMaxWidth()).c_str(), __FILE__, __LINE__);
	Herve::utilInfo("maxHeight = ", std::to_string(gUptrDisplayDevice->getMaxHeight()).c_str(), __FILE__, __LINE__);


	//////////////////////////////////////////////////////////////////////
	// main loop
	//////////////////////////////////////////////////////////////////////

	SDL_Event event;

	while (true) {

		// handle events
		while( SDL_PollEvent( &event ) ) {
			switch(event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYUP:
				keyUp(event.key);
				break;
			case SDL_VIDEORESIZE :
				resize(event.resize);
				break;
			}
		}

		// refresh window
		render();
	}

	return 0;
}
