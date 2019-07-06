#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "MatrixVecs.h"
#include "Assets.h"
#include "Dome.h"
#include "Lights.h"
#define _USE_MATH_DEFINES
#define M_PI       3.14159265358979323846

class AbstractComponent {
public:
	AffineMap am;
	fp mass;
	fp moi;

	AbstractComponent(Vector3 displacementV, fp Moi, fp Mass);
	AbstractComponent() {};
	AbstractComponent(Vector3 displacementV, fp ang, char axis, fp Moi, fp Mass);

	virtual void render() = 0;
	void ApplyAffine();
};

class Rocket :public AbstractComponent {
public:
	AffineMap iam;
	fp throttle = 1.0;
	fp force = 60.0;
	bool IsOn = false;
	bool IsBig = false;
	lightClass *light;

	Rocket() : AbstractComponent() {};
	Rocket(Vector3 displacementV, char axis, fp angle, fp mass, lightClass *Light) : AbstractComponent(displacementV, angle, axis, 0, mass) {
		iam = am.invert();
		light = Light;
	};

	void render();
	Vector3 GetTorque();
	Vector3 GetForce();
};

class PhysModel {
public:
	AffineMap am;
	AffineMap iam;
	Vector3 VelLinear;
	Vector3 VelAngular;
	bool IsStarted = false; 
	fp mass;
	fp moi;
	fp radius;
	fp height;

	fp fRate;

	DomeClass dome;
	GLUquadricObj *quadric1 = gluNewQuadric();

	AbstractComponent** components;
	int numComs;

	Rocket** rockets;
	int numRocs;

	//AbstractComponent** ro;

	PhysModel() {
	}

	PhysModel(fp x, fp y, fp z, int frameRate, fp Radius,fp Height) {
		am = AffineMap(x,y,z);
		VelLinear = Vector3();
		VelAngular = Vector3();
		fRate = (fp)frameRate;

		fp density = 2.0;
		radius = Radius;
		height = Height;

		mass = M_PI * 4 / 3 * radius * radius * radius;
		moi = M_PI * 5 / 2 * radius * radius * radius;
	}

	void update();
	void updateVelocities();
	void setComponents(AbstractComponent** coms, int num);
	AbstractComponent* GetComponent(int num);
	void setRockets(Rocket** rocs, int num);
	void ApplyAffine();
	void TurnRocketsOff();

	void render();
};

