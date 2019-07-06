#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "OnRails.h"
#include "Dome.h"
#include "CubeClass.h"

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class SolarPanel1 :public OnRails {
public:
	float scaleFactor = 1.0;
	GLUquadricObj *quadric1 = gluNewQuadric();
	DomeClass *dome = new DomeClass(20, 20, 5, 1);

	SolarPanel1(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime);

	void renderSelf();
};

class SolarPanelSmall :public OnRails {
public:
	float scaleFactor = 1.0;


	SolarPanelSmall(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime) {
		IsStarted = false;
		Restart = false;
		StartChildrenOnStart = true;
		StartChildrenOnFinish = false;
	};

	void renderSelf();
};

class Lid :public OnRails {
public:
	float scaleFactor = 1.0;
	CubeClass *cube = new CubeClass();

	Lid(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime) {
		IsStarted = false;
		Restart = false;
	};

	void renderSelf();
};

class Delay :public OnRails {
public:
	float scaleFactor = 1.0;

	Delay(float frameRate, float RunningTime) :OnRails(0, 'n', 0, 0, 0, frameRate, RunningTime) {
		IsStarted = false;
		Restart = false;
		StartChildrenOnStart = false;
		StartChildrenOnFinish = true;
	};

	void renderSelf();
};

class SolarPanel2 :public OnRails {
public:
	float scaleFactor = 1.0;
	CubeClass *cube = new CubeClass();
	AffineMap am;

	SolarPanel2(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime);

	void renderSelf();
};

class Hab1 :public OnRails {
public:
	float scaleFactor = 1.0;
	GLUquadricObj *quadric1 = gluNewQuadric();
	DomeClass *dome = new DomeClass(20, 20, 5, 1);
	fp radius = 4;
	fp height = 5;

	Hab1(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime) { Restart = false; };

	void renderSelf();
};

class FuelPump :public OnRails {
public:
	float scaleFactor = 1.0;

	FuelPump(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime) {};

	void renderSelf();
};

void SetBrass();
void SetPewter();
void SetJade();
void SetRedPlas();
void SetOther();
void SetChrome();
void SetWhite();
void SetDirt();
void SetBluePlas();
void SetBlueFlame();
void SetRedFlame();
void SetYellowSand();
void SetAl();