#ifndef __Terrain_grid_ZZ__
#define __Terrain_grid_ZZ__
// terrain.h 

#include <windows.h>
#include "Lights.h"
#include "fileOperations.h"

#define delta = 35;

class terrain 
{
protected:

GLfloat xsize;
GLfloat zsize;
int xsteps;
int zsteps;

GLfloat map[MAXWIDTH][MAXDEPTH];
int mapInt[MAXWIDTH][MAXDEPTH];
vec3 pts[MAXWIDTH][MAXDEPTH];
vec3 normals[MAXWIDTH][MAXDEPTH];

public:

int mapWidth, mapHeight;

materialClass* mat;
materialClass* matLines;

materialClass* matHigh;
materialClass* matBeach;
materialClass* matLand;

GLfloat highHeight;
GLfloat grassHeight;

void setUp(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ);
void setMaterialHeight(GLfloat height);
void setPts();
void setNormals();
vec3 normal(int row, int col);
void setNormal(int row, int col);

virtual int render(float tx, float ty, float tz, float amx, float amy, float amz);
virtual int renderLines();
terrain();
~terrain();

};


#endif
