// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "Scene.hpp"

#include <GL/glut.h>
#include <iomanip>
#include <iostream>
#include <sstream>

// window data
bool gFullscreen = false;
bool gShowHud = true;
int gDefaultWindowWidth = 640;
int gDefaultWindowHeight = 360;

// scene data
Scene gScene;
Herve::DisplayDeviceMono gDisplayDevice;
Herve::FpsCounter gFpsCounter;
float gFps = 0;
glm::vec3 gRelativePosition;
glm::vec3 gRelativeVelocity;

// translations (keyboard)
Herve::InputMotion1dRelative gKeyForward(4);
Herve::InputMotion1dRelative gKeyBackward(4);
Herve::InputMotion1dRelative gKeyLeft(3);
Herve::InputMotion1dRelative gKeyRight(3);
Herve::InputMotion1dRelative gKeyUp(2);
Herve::InputMotion1dRelative gKeyDown(2);

// orientation (mouse)
Herve::InputMotion2d gMouse(0, 0, -0.002, -0.002);

void switchFullscreen() {
	gFullscreen = not gFullscreen;
	if (gFullscreen) {
		gDefaultWindowWidth = gDisplayDevice.getWidth();
		gDefaultWindowHeight = gDisplayDevice.getHeight();
		glutFullScreen();
	}
	else {
		glutReshapeWindow(gDefaultWindowWidth, gDefaultWindowHeight);
	}
}

// display text using OpenGL
// clears the stream oss
void renderText(std::stringstream & oss, int x, int y, int w, int h) {

	// set projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);

	// set modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// render bitmap text
	glRasterPos3f(x, y, 0);
	const char * text = oss.str().c_str();
	for (const char * c=text; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
	oss.str( std::string() );
	oss.clear();

	// restore projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// restore modelview
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void processDisplay() {

	// render the scene
	gDisplayDevice.render(&gScene);

	// display info
	if (gShowHud) {
		glColor4d(1, 1, 1, 1);
		std::stringstream oss;
		int width = gDisplayDevice.getWidth();
		int height = gDisplayDevice.getHeight();

		gFpsCounter.countNewFrame();
		if (gFpsCounter.hasFps()) {
			gFps = gFpsCounter.getFpsAndRestart();
			gScene.getRelativePositionAndVelocity(gRelativePosition, gRelativeVelocity);
		}

		oss << "FPS = " << (int)gFps;
		renderText(oss, 20, height-60, width, height);

		oss << "Relative position = " << std::fixed << std::setprecision(1)
			<< gRelativePosition[0] << ", " << gRelativePosition[1] << ", " << gRelativePosition[2];
		renderText(oss, 20, height-40, width, height);

		oss << "Relative velocity = " << std::fixed << std::setprecision(1)
			<< gRelativeVelocity[0] << ", " << gRelativeVelocity[1] << ", " << gRelativeVelocity[2];
		renderText(oss, 20, height-20, width, height);

		oss << "3D sound (t) = " << (gScene.getHasSound3d() ? "on" : "off");
		renderText(oss, 20, 20, width, height);

		oss << "doppler (y) = " << (gScene.getHasSoundDoppler() ? "on" : "off");
		renderText(oss, 20, 40, width, height);

		oss << "reverb (u) = " << (gScene.getHasSoundReverb() ? "on" : "off");
		renderText(oss, 20, 60, width, height);

		oss << "sound (i) = " << (gScene.getHasSound() ? "on" : "off");
		renderText(oss, 20, 80, width, height);

		oss << "music (o) = " << (gScene.getHasMusic() ? "on" : "off");
		renderText(oss, 20, 100, width, height);

		oss << "fullscreen (f) = " << (gFullscreen ? "on" : "off");
		renderText(oss, 20, 120, width, height);

		oss << "use arrow/page/esc keys" ;
		renderText(oss, 20, 140, width, height);
	}

	glutSwapBuffers();
}

void processReshape(int w, int h) {
	gDisplayDevice.resizeWindow(w, h);
}

void processIdle() {

	// update camera
	float tx = gKeyRight.getValue() - gKeyLeft.getValue();
	float ty = gKeyUp.getValue() - gKeyDown.getValue();
	float tz = gKeyBackward.getValue() - gKeyForward.getValue();
	float rx = gMouse.getYValue();	// mouse y controls rotation around opengl x-axis
	float ry = gMouse.getXValue();	// mouse x controls rotation around opengl y-axis
	gDisplayDevice.updateView(tx, ty, tz, rx, ry, 0.f);

	glutPostRedisplay();
}

void processMotion(int x, int y) {
	gMouse.motion(x, y);
}

void processMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) gMouse.press(x, y);
		else gMouse.release(x, y);
	}
}

void processSpecialDown(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP : gKeyForward.press(); break;
	case GLUT_KEY_DOWN : gKeyBackward.press(); break;
	case GLUT_KEY_PAGE_DOWN : gKeyDown.press(); break;
	case GLUT_KEY_PAGE_UP : gKeyUp.press(); break;
	case GLUT_KEY_LEFT : gKeyLeft.press(); break;
	case GLUT_KEY_RIGHT : gKeyRight.press(); break;
	}
}

void processSpecialUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP : gKeyForward.release(); break;
	case GLUT_KEY_DOWN : gKeyBackward.release(); break;
	case GLUT_KEY_PAGE_DOWN : gKeyDown.release(); break;
	case GLUT_KEY_PAGE_UP : gKeyUp.release(); break;
	case GLUT_KEY_LEFT : gKeyLeft.release(); break;
	case GLUT_KEY_RIGHT : gKeyRight.release(); break;
	}
}

void processKeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 27 : exit(0); break;
	case 'f' : switchFullscreen(); break;
	case 'r' : gShowHud = not gShowHud; break;
	case 't' : gScene.switchSound3d(); break;
	case 'y' : gScene.switchSoundDoppler(); break;
	case 'u' : gScene.switchSoundReverb(); break;
	case 'i' : gScene.switchSound(); break;
	case 'o' : gScene.switchMusic(); break;
	}
}

int main(int argc, char **argv) {

	gDisplayDevice.initDevice();

	// initialize glut window
	glutInit(&argc, argv);
	glutInitWindowSize(gDefaultWindowWidth, gDefaultWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutCreateWindow(argv[0]);

	// initialize callback functions
	glutDisplayFunc(processDisplay);
	glutReshapeFunc(processReshape);
	glutIdleFunc(processIdle);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMotion);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardUpFunc(processKeyboardUp);
	glutSpecialFunc(processSpecialDown);
	glutSpecialUpFunc(processSpecialUp);

	// initialize scene data
	gDisplayDevice.initDisplay(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	gScene.init(&gDisplayDevice);
	gFpsCounter.start();

	// launch main loop
	glutMainLoop();

	return 0;
}
