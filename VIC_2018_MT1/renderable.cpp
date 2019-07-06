// renderable.cpp
#include <windows.h>
#include "renderable.h"

void renderableParent::setTranslate(GLfloat xx, GLfloat yy, GLfloat zz)
{
xPos=xx;
yPos=yy;
zPos=zz;
}

void renderableParent::setRot(GLfloat xx, GLfloat yy, GLfloat zz)
{
xRot=xx;
yRot=yy;
zRot=zz;
}

void renderableParent::setScale(GLfloat xx, GLfloat yy, GLfloat zz)
{
xScale=xx;
yScale=yy;
zScale=zz;
}

void renderableParent::preset() // presets rotation and position and scale
{
doTranslate();
doRotate();
doScale();
}

void renderableParent::init()
{
xScale=1;
yScale=1;
zScale=1;
xPos=0;
yPos=0;
zPos=0;
xRot=0;
yRot=0;
zRot=0;
}

void renderableParent::doTranslate()
{
glTranslatef(xPos, yPos, zPos);
}

void renderableParent::doScale()
{
glScalef(xScale, yScale, zScale);
}

void renderableParent::doRotate()
{
	if (xRot!=0) glRotatef(xRot,1,0,0);
	if (yRot!=0) glRotatef(yRot,0,1,0);
	if (zRot!=0) glRotatef(zRot,0,0,1);
}

void renderableParent::undoRotate()
{
	if (zRot!=0) glRotatef(-zRot,0,0,1);	
	if (yRot!=0) glRotatef(-yRot,0,1,0);
	if (xRot!=0) glRotatef(-xRot,1,0,0);
}

renderableParent::renderableParent()
{
	init();
}

// end
