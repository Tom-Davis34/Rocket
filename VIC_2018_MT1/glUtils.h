
#ifndef _glUtils_
#define _glUtils_

#include "utility.h"

typedef GLfloat UC_RGBA[4];
typedef GLfloat Vec3f[3];
typedef GLfloat Vec4f[4];

extern UC_RGBA White;
extern UC_RGBA Black;

/* Alpha is set to 1 by default */
extern void SetColor (UC_RGBA color, GLfloat r, GLfloat g, GLfloat b);
extern void CopyColor (UC_RGBA dest, UC_RGBA source);

extern void SetVec (Vec3f dest, float x, float y, float z);
extern void AddVec (Vec3f dest, Vec3f v1, Vec3f v2);
extern void NormVec (Vec3f dest);

extern void CheckGL (void);

extern void BitmapString (void * font, float x, float y, String msg);

extern float Min (float f, float limit);
extern float Max (float f, float limit);
extern float Clamp (float f, float min, float max);

#endif
