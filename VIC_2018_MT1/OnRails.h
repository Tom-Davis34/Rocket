#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MatrixVecs.h"
#include "Dome.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class OnRails {
public:
	float ang;
	char axis;
	float transX;
	float transY;
	float transZ;

	float RunTime = 0.0;
	float MaxTime;
	float fRate;
	bool IsStarted = true;
	bool Restart = false;
	bool StartChildrenOnFinish = true;
	bool StartChildrenOnStart = false;;

	OnRails **children = new OnRails*[100];
	int numChildren = 0;

	OnRails(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime);
	OnRails(float frameRate);

	void ApplyAffine();
	void render();
	virtual void renderSelf() = 0;
	void addChild(OnRails *child);
	void StartChildren();
	void Start();
	void updateTime();
	bool IsFinished();
};

class StaticOnRails : OnRails{
	StaticOnRails(float an, char ax, float translationX, float translationY, float translationZ, float frameRate) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, 0) {};
	void renderSelf() {};
};