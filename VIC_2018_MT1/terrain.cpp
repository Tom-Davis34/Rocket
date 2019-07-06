// terrain.cpp

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "terrain.h"
#include "fileOperations.h"
#include "vec3.h"

// -----------------------------------------------------------------------------------

// X22_0112.TXT

//GLfloat xsize;
//GLfloat zsize;
//int xsteps;
//int zsteps;

//materialClass* mat;
//materialClass* matLines;

void terrain::setUp(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ)
{
xsize=xsizeZ;
zsize=zsizeZ;
xsteps=xstepsZ;
zsteps=zstepsZ;
}

void terrain::setMaterialHeight(GLfloat height)
{
//	materialClass* matHigh;
//materialClass* matBeach;
//materialClass* matLand;

//GLfloat highHeight;
//GLfloat grassHeight;
	if (height >= highHeight) 
		matHigh->doMaterial(); 
	else 
	{
		if (height > grassHeight) 
		     matLand->doMaterial(); 
	     else
			 matBeach->doMaterial();
	}
}

void terrain::setPts()
{
	GLfloat xdelta = xsize / xsteps;
	GLfloat zdelta = zsize / zsteps;
	for (int x = 0; x<xsteps; x++){
		for (int z = 0; z < zsteps; z++) {
			pts[x][z] = vec3(x*xdelta, map[x][z], z*zdelta);
		}
	}
}

void terrain::setNormals()
{
	for (int x = 0; x<xsteps; x++) {
		for (int z = 0; z < zsteps; z++) {
			normals[x][z] = normal(x, z);
		}
	}
}

vec3 terrain::normal(int x, int z) {
	vec3 norm = vec3();
	int count = 0;

	if (x > 0 && z > 0) {
		count++;
		add(norm, norm, cross(pts[x - 1][z] - pts[x][z], pts[x][z - 1] - pts[x][z]));
	}
	if (x > 0 && z < zsteps) {
		count++;
		add(norm, norm, cross( pts[x][z + 1] - pts[x][z], pts[x - 1][z] - pts[x][z]));
	}
	if (x < xsteps && z > 0) {
		count++;
		add(norm, norm, cross( pts[x][z - 1] - pts[x][z], pts[x + 1][z] - pts[x][z]));
	}
	if (x < xsteps && z < zsteps) {
		count++;
		add(norm, norm, cross(pts[x + 1][z] - pts[x][z], pts[x][z + 1] - pts[x][z]));
	}

	norm = norm*(-1.0 / length(norm));
	return norm;
}

void terrain::setNormal(int row, int col)
{
	glNormal3f(normals[row][col].x, normals[row][col].y, normals[row][col].z);
}

int terrain::render(float tx, float ty, float tz, float amx, float amy, float amz)
{ 
glPushMatrix();


GLfloat xdelta=xsize/xsteps;
GLfloat zdelta=zsize/zsteps;
   

for (int x=0; x<xsteps-1; x++)
	for (int z=0; z<zsteps-1; z++)
	{
		float mx = -xdelta * x - tx;
		float mz = -zdelta * z -tz;

		if ((amx - mx)*(amx - mx) + (amz - mz)*(amz - mz) > 90000)
			continue;

	glBegin(GL_TRIANGLES);
    setMaterialHeight(map[x][z]);
	setNormal(x, z);
	glVertex3f(xdelta*x,map[x][z],zdelta*z);
	setMaterialHeight(map[x+1][z]);
	setNormal(x + 1, z);
	glVertex3f(xdelta*x+xdelta,map[x+1][z],zdelta*z);
	setMaterialHeight(map[x][z+1]);
	setNormal(x, z + 1);
	glVertex3f(xdelta*x,map[x][z+1],zdelta*z+zdelta);	
	glEnd();

    glBegin(GL_TRIANGLES);
	setMaterialHeight(map[x+1][z+1]);
	setNormal(x + 1, z + 1);
	glVertex3f(xdelta*x+xdelta,map[x+1][z+1],zdelta*z+zdelta);	
	setMaterialHeight(map[x][z+1]);
	setNormal(x, z + 1);
	glVertex3f(xdelta*x,map[x][z+1],zdelta*z+zdelta);		
	setMaterialHeight(map[x+1][z]);
	setNormal(x + 1, z);
	glVertex3f(xdelta*x+xdelta,map[x+1][z],zdelta*z);
	glEnd();
	}

//mat->setMaterialBlue();
//mat->doMaterial();
//for (int x=0; x<xsteps-1; x++)
//	for (int z=0; z<zsteps-1; z++)
//	{
//	glVertex3f(xdelta*x,0.1,zdelta*z);
//	glVertex3f(xdelta*x+xdelta,0.1,zdelta*z);
//	glVertex3f(xdelta*x,0.1,zdelta*z+zdelta);	
//	
//	glVertex3f(xdelta*x+xdelta,0.1,zdelta*z+zdelta);	
//	glVertex3f(xdelta*x,0.1,zdelta*z+zdelta);		
//	glVertex3f(xdelta*x+xdelta,0.1,zdelta*z);
//	}

 
glPopMatrix();
return true;
}

int terrain::renderLines()
{
glPushMatrix();

matLines->doMaterial();

GLfloat xdelta=xsize/xsteps;
GLfloat zdelta=zsize/zsteps;

glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

glBegin(GL_TRIANGLES);    
glNormal3f(0,1,0); // yes veronica all normals point up (students need to fix this)
for (int x=0; x<xsteps-1; x++)
	for (int z=0; z<zsteps-1; z++)
	{

	glVertex3f(xdelta*x,0.01+map[x][z],zdelta*z);
	glVertex3f(xdelta*x+xdelta,0.01+map[x+1][z],zdelta*z);
	glVertex3f(xdelta*x,0.01+map[x][z+1],zdelta*z+zdelta);	
	
	glVertex3f(xdelta*x+xdelta,0.01+map[x+1][z+1],zdelta*z+zdelta);	
	glVertex3f(xdelta*x,0.01+map[x][z+1],zdelta*z+zdelta);		
	glVertex3f(xdelta*x+xdelta,0.01+map[x+1][z],zdelta*z);
	}

glEnd();  
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glPopMatrix();
return true;
}

terrain::terrain()
{
mat= new materialClass();
matLines= new materialClass(); 
mat->setMaterialJade();
matLines->setMaterialBlackRubber();

matHigh=new materialClass();
matBeach=new materialClass();
matLand=new materialClass();

matHigh->setMaterialRedPlastic();
matBeach->setMaterialYellowSand();
matLand->setMaterialGreenGrass();

//mapWidth=6;
//mapHeight=6;
//
//GLfloat m0[]={0,0,0,0,0,0};
//GLfloat m1[]={0,0.2,0.3,0.1,0.2,0};
//GLfloat m2[]={0,1,1.5,3,2,0};
//GLfloat m3[]={0,1,2,1.5,2,0};
//GLfloat m4[]={0,1,0.4,0.2,0.2,0};
//GLfloat m5[]={0,0,0,0,0,0};
//for (int i=0; i<6; i++) map[0][i]=m0[i];
//for (int i=0; i<6; i++) map[1][i]=m1[i];
//for (int i=0; i<6; i++) map[2][i]=m2[i];
//for (int i=0; i<6; i++) map[3][i]=m3[i];
//for (int i=0; i<6; i++) map[4][i]=m4[i];
//for (int i=0; i<6; i++) map[5][i]=m5[i];

readHeightField (findFile("hf.txt"), &mapWidth, &mapHeight, mapInt);
for (int x=0; x<mapWidth; x++)
	{
	for (int y=0; y<mapHeight; y++) 
		{
		map[x][y]=((float)mapInt[x][y])/5;
		}
	}

int scale = 7.5;
setUp(mapWidth * scale, mapHeight * scale,mapWidth,mapHeight);

float scale2 = 6;
highHeight=2.8 * scale2;
grassHeight=1.0 * scale2;
}

terrain::~terrain()
{
delete mat;
delete matLines;
delete matBeach;
delete matHigh;
delete matLand;
}

// end


