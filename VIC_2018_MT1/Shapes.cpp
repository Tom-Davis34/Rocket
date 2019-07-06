// shapes.cpp
#include <windows.h>
#include "shapes.h"

glutObject::glutObject()
{
	init();
}

glutObject::glutObject(objKind k)
{
	init();
	kind=k;
}

glutObject::~glutObject()
{
	gluDeleteQuadric(quadric1);
}

void glutObject::init()
{
    // some 'sane' defaults
	red=0.5; green=0.7; blue=0.3; // puse green colour
	size=1;
	radius=0.5;
	radiusBig=1;
	stackz=11;
	slicez=11;
	setKind(cubeK); //{sphereK, cubeK, coneK, cylinderK, teapotK, diskK, cylinderTaperK, torusK};
	quadric1=gluNewQuadric();
	setWireFrame(true);
}

void glutObject::setColour(GLfloat r, GLfloat g, GLfloat b)
{
	red=r;
	green=g;
	blue=b;
}

void glutObject::setSize(GLfloat s)
{
	size=s;
}

void glutObject::setStacksSlices(int slices, int stacks)
{
	stackz=stacks;
	slicez=slices;
}

void glutObject::setRadius(GLfloat radiuz)
{
	radius=radiuz;
}

void glutObject::setRadii(GLfloat radiusSmall, GLfloat radiusLarge)
{
	radius=radiusSmall;
	radiusBig=radiusLarge;
}

void glutObject::setWireFrame(bool w)
{
	wireframe=w;
	setQuadMode();
	}

void glutObject::setQuadMode()
{
	if (wireframe) gluQuadricDrawStyle(quadric1, GLU_LINE); //wireframe mode
	else gluQuadricDrawStyle(quadric1, GLU_FILL);
	gluQuadricNormals(quadric1, GLU_SMOOTH);
	gluQuadricOrientation(quadric1,GLU_OUTSIDE);

}

void glutObject::setKind(objKind k)
{
	kind=k;
}

int glutObject::render()
{
glColor3f(red, green, blue);

glPushMatrix();
preset();

if (wireframe)
{
	switch (kind)
	{
	case sphereK : glutWireSphere(radius, slicez, stackz);
		break;
	case cubeK : glutWireCube(size);
		break;
	case coneK : glutWireCone(radius, size, slicez, stackz);
		break;
	case cylinderK : setQuadMode(); gluCylinder(quadric1, radius, radius, size, slicez, stackz);
		break;
	case teapotK :  glutWireTeapot(size);
		break;
	case diskK : setQuadMode(); gluDisk(quadric1, radius, radiusBig, slicez, stackz);
		break;
	case cylinderTaperK :   setQuadMode(); gluCylinder(quadric1, radius, radiusBig, size, slicez, stackz);
		break;
	case torusK : glutWireTorus(radius, radiusBig, slicez, stackz);
		break;
		
	}
}
else
{
    switch (kind)
	{
	case sphereK : glutSolidSphere(radius, slicez, stackz);
		break;
	case cubeK : glutSolidCube(size);
		break;
	case coneK : glutSolidCone(radius, size, slicez, stackz);
		break;
	case cylinderK : gluCylinder(quadric1, radius, radius, size, slicez, stackz);
		break;
	case teapotK : glutSolidTeapot(size);
		break;
	case diskK :setQuadMode(); gluDisk(quadric1, radius, radiusBig, slicez, stackz);
		break;
	case cylinderTaperK :setQuadMode(); gluCylinder(quadric1, radius, radiusBig, size, slicez, stackz);
		break;
	case torusK : glutSolidTorus(radius, radiusBig, slicez, stackz);
		break;		
	}

}

glPopMatrix();
return true;
}

// end