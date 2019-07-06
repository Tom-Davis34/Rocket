#ifndef __SHAPES_Z__
#define __SHAPES_Z__

#include "renderable.h"

class glutObject : public renderableParent
{	
public:
enum objKind {sphereK, cubeK, coneK, cylinderK, teapotK, diskK, cylinderTaperK, torusK}; 

protected:
	GLUquadricObj *quadric1;
	GLfloat red,green,blue;
	GLfloat size;
	bool wireframe; // true or false
	GLfloat radius, radiusBig;
	objKind kind;
    int stackz, slicez;

	void init();
	void setQuadMode();

public:

void setColour(GLfloat r, GLfloat g, GLfloat b);
void setSize(GLfloat s);
void setStacksSlices(int slices, int stacks);
void setRadius(GLfloat radius);
void setRadii(GLfloat radiusSmall, GLfloat radiusLarge);
void setWireFrame(bool w);
void setKind(objKind k);

virtual int render();

glutObject();
glutObject(objKind k);
~glutObject();

};

#endif
