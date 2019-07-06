#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

typedef double fp;

class Vector3
{
public:
	fp v[3] = { 0,0,0 };

	Vector3();
	Vector3(fp x, fp y, fp z); // constructor
												 //void render(); // draw it
	fp Length();
	Vector3 cross(Vector3);
	fp dot(Vector3);
	Vector3 add(Vector3);
	Vector3 sub(Vector3);
	Vector3 scale(fp s);
	fp &operator () (int arg);
};

class Matrix33
{
public:
	fp Matrix[3][3] = { { 1,0,0 },{ 0,1,0 } ,{ 0,0,1 } };

	Matrix33() {
	}

	Matrix33(fp ang, char c);
	Matrix33(fp angX, fp angY, fp angZ);

	void RotX(fp ang);
	void RotZ(fp ang);
	Matrix33 RotByX(fp ang);
	Matrix33 RotByZ(fp ang);
	Vector3 Multiply(Vector3 b);
	Vector3 Multiply(Vector3 b, fp s);
	Matrix33 Multiply(Matrix33 b);
	Matrix33 Multiply(Matrix33 m, fp s);
	fp det();
	void scale(fp s);
	Matrix33 invert();
	fp &operator ()(int arg1, int arg2);
};


class AffineMap {
public:
	Matrix33 *M;
	Vector3 *Tr;

	AffineMap() {
		M = new Matrix33();
		Tr = new Vector3();
	}

	AffineMap(fp x, fp y, fp z) {
		M = new Matrix33();
		Tr = new Vector3(x,y,z);
	}

	AffineMap(fp x, fp y, fp z, fp ang, char c) {
		M = new Matrix33(ang, c);
		Tr = new Vector3(x, y, z);
	}

	void RotX(fp ang);
	void RotZ(fp ang);


	AffineMap invert();
	Vector3 transform(Vector3 v);
	Vector3 rotate(Vector3 v);
	void update(AffineMap am, fp scale);
	void updateTr(Vector3 vec);
	const float* getMap();
};