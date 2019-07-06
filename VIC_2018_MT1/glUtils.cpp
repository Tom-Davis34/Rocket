
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

#include "utility.h"

#include "glUtils.h"

/****		Handy types		****/

UC_RGBA White = { 1, 1, 1, 1 };
UC_RGBA Black = { 0, 0, 0, 1 };

void SetColor (UC_RGBA color, GLfloat r, GLfloat g, GLfloat b)
{
  color[0] = Clamp(r, 0, 1);
  color[1] = Clamp(g, 0, 1);
  color[2] = Clamp(b, 0, 1);
  color[3] = 1;
}

void CopyColor (UC_RGBA dest, UC_RGBA source)
{
  int i;

  for (i = 0; i < 4; i++)
    dest[i] = source[i];
}

void SetVec (Vec3f dest, float x, float y, float z)
{
  dest[0] = x;
  dest[1] = y;
  dest[2] = z;
}

void AddVec (Vec3f dest, Vec3f v1, Vec3f v2)
{
  dest[0] = v1[0] + v2[0];
  dest[1] = v1[1] + v2[1];
  dest[2] = v1[2] + v2[2];
}

void NormVec (Vec3f dest)
{
  float len;
  
  len = sqrtf(dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2]);
  if (len > 0) {
    dest[0] /= len;
    dest[1] /= len;
    dest[2] /= len;
  }
}

/****		Misc utilities		****/

/****	Font utilities	****/

void BitmapString (void * font, float x, float y, String msg)
{
  int i;

  FailNull(font, "BitmapString: no font set");

  /* Use the WindowPos extension originally from Mesa so we
     always get window coords without having to set up a
     special ortho projection */
  //glWindowPos2f(x, y);
  glRasterPos2f(x,y);
  for (i = 0; i < strlen(msg); i++)
    glutBitmapCharacter(font, msg[i]);
}


/****	Handy math	****/

float Min (float f, float limit)
{
  if (f > limit)
    return limit;
  else
    return f;
}

float Max (float f, float limit)
{
  if (f < limit)
    return limit;
  else
    return f;
}

float Clamp (float f, float min, float max)
{
  if (f < min)
    return min;
  else if (f > max)
    return max;
  else
    return f;
}


