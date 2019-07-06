
/****	First 3D program : the spinning cube
	Written by Rob Cox and Hugh Fisher				****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "Axes.h"
#include "CubeClass.h"
#include "Dome.h"
#include "2DText.h"
#include "grid.h"
#include "MatrixVecs.h"
#include "PhysModel.h"
#include "vec3.h"
#include "utility.h"
#include "texturePoly.h"
#include "Shapes.h"
#include "Renderable.h"
#include "Bitmap.h"
#include "terrain.h"
# include "PSystem.h"
#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 30
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame=false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement = 0; // slow

static GLfloat	eyePoint[3];
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;
vec3    eyePos;
int     timerInterval = timerMSecs;
static GLfloat	color3[3];

static GLfloat	red[]		= {1.0, 0.0, 0.0};
static GLfloat	blue[]		= {0.0, 1.0, 0.0};
static GLfloat	green[]		= {0.0, 0.0, 1.0};
static GLfloat	cyan[]		= {0.0, 1.0, 1.0};
static GLfloat	magenta[]	= {1.0, 0.0, 1.0};
static GLfloat	yellow[]	= {1.0, 1.0, 0.0};
static GLfloat	black[]		= {0.0, 0.0, 0.0};
static GLfloat	white[]		= {1.0, 1.0, 1.0};

CubeClass *cube; // pointer to a cube object
GLUquadricObj *quadric1 = gluNewQuadric();
GLUquadricObj *quadric2;  
GLUquadricObj *quadric3;    
GLUquadricObj *quadric4;    
Axes *axes; //
Grid *grid; // To help with push and scal etc
DomeClass *dome0;
DomeClass *dome1;
DomeClass *dome2;
DomeClass *domeHab;

lightClass * lightSun;
lightClass * lightRocB;
lightClass * lightRocS1;
lightClass * lightRocS2;
lightClass * lightSpot;

int showGrid=true;
int showAxes=true;
int showBase1=true;
int showBase2=false;
int showWireFrame=false;
int showSpin=false;
int showChrisTrapani=false; // change center of spin 

int hiddenDepth=true;
int hiddenCull=false;
int clockWise=false; // start CCW
GLfloat angle = 0;
GLfloat angleVar = 0;

GLfloat centerX = 10;
GLfloat centerZ = 10;
int viewMode = 0;
int mode=3; // mode 2 = 2D, 3=3D , 5=Both
Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

CBitmap *texTemp;
GLubyte tex1[128][128][4];


// Function prototypes needed because they are used before its declared

//static int err ;
GLenum __stdcall err1;
static GLfloat ambientIntensity01[] = { 0.3f, 0.3f ,0.3f ,1.0f }; // RGBA color for ambient
static GLfloat ambientIntensity0[] = { 0.0f, 0.0f, 0.0f ,1.0f }; // RGBA color for ambient
static bool ambientLight = false;
static bool eLight = true;
static bool eLight0 = true;

PhysModel* pm;
Rocket** rocs;
SolarPanel2 *sp2;
SolarPanel2 *sp1;
SolarPanel1 *solar1;

tex_quad *tex_q;
tex_triangle *tex_t;
CBitmap *bmp0; // the texture 
GLuint texHandle; // the open gl texture handle
GLfloat fogColour[4] = { 0.8, 0.4, 0.2, 1.0 };
bool dofog = true;
float skySphereRad = 300;
DomeClass *skySphere = new DomeClass(36, 36, 5, 300);
float terrX = -200;
float terrY = -43;
float terrZ = -200;

terrain *terr;
void CheckGL()
{
	int  err;
	char msg[256];

	err = glGetError();
	if (err != GL_NO_ERROR) {
		sprintf(msg, "OpenGL error: %s", (GLubyte*)gluErrorString(err));
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

/****		Window events		****/

void renderHab(float x, float z) {

	float radius = 5;
	float height = 4;
		glPushMatrix();
		//glLoadIdentity();
		glTranslatef(x, height + 3, z);
		SetWhite();
		//render cylinder
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glColor3f(0.8, 0.8, 0.8);
		glTranslatef(0, 0, -height * 0.5);
		glScalef(radius, radius, height);
		gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 4);
		glPopMatrix();
	
		//render cylinder
		glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, -radius - height * 0.5, 0);
		domeHab->renderWithNormalsOut();
		glPopMatrix();
		
		//render legs
		SetChrome();
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glColor3f(0.1, 0.1, 0.1);
	
		glPushMatrix();
		glTranslatef(radius*0.8, 0, height * 0.5);
		gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
		glPopMatrix();
	
		glPushMatrix();
		glTranslatef(-radius * 0.8, 0, height * 0.5);
		gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
		glPopMatrix();
	
		glPushMatrix();
		glTranslatef(0, -radius * 0.8, height * 0.5);
		gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
		glPopMatrix();
	
		glPushMatrix();
		glTranslatef(0, radius * 0.8, height * 0.5);
		gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void renderSolar(float x) {
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(x, 1, 75);

	solar1->render();

	glPopMatrix();
}

void enableLighting()
{
	if (true) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
	if (false) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	if (true)
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity01[0]);
		err1 = glGetError();
	}
	else
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity0[0]);
		err1 = glGetError();
	}
	if (err1 != GL_NO_ERROR) printf("err1=%d\n", (int)err1);
}

void setUpLights()
{
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	//int lpos = 3;
	//float	lpos1[] = { 3,  3, 0.0, 1.0 }; // point source close 
	//float	lpos2[] = { 100,  6, 100, 1.0 }; // point source 
	//float	lpos3[] = { 140, 140, 0.0, 0.0 }; // distant light
	//float	lamb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//float	lamb0[] = { 0.0f, 0.01f, 0.0f, 1.0f };
	//float	ldif[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	//float	lspe[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	//
	//float	ldifror[] = { 6, 6, 6, 1.0f };

	//Vector3 vel = pm->am.M->Multiply(Vector3(0, -6.5, 0));
	//vel = vel.add(*(pm->am).Tr);
	//float lposRoc[] = {vel.v[0],vel.v[1], vel.v[2],1.0f};
	//if (lpos == 1) glLightfv(GL_LIGHT0, GL_POSITION, lpos1);
	//if (lpos == 2) glLightfv(GL_LIGHT0, GL_POSITION, lpos2);
	//if (lpos == 3) glLightfv(GL_LIGHT0, GL_POSITION, lpos3);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lspe);

	//glLightfv(GL_LIGHT1, GL_POSITION, lposRoc);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, lamb);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lspe);

	////glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.3);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,0.09);

	//glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	lightRocB->letThereBeDark();
	lightRocS1->letThereBeDark();
	lightRocS2->letThereBeDark();

	lightSun->letThereBeLight();
	glPushMatrix();
	glTranslatef(70, 16, 100);
	glRotatef(spinAngle,0,1,0);
	glRotatef(-25, 1, 0, 0);
	glTranslatef(0, 0, 0.5);
	lightSpot->letThereBeLight();
	glTranslatef(0, 0, -0.5);
	glRotatef(180, 0, 1, 0);

	//glRotatef(30, 0, 0, 1);
	gluCylinder(quadric1, 0.0, 0.5, 1, 18, 6);
	glRotatef(65, 1, 0, 0);
	gluCylinder(quadric1, 0.2, 0.2, 5, 18, 6);
	glPopMatrix();



	//enableLighting();
}

static void seteyePoint()
{
  glLoadIdentity();
  if(viewMode ==0)
  gluLookAt((pm->am).Tr->v[0] + eyePoint[0], (pm->am).Tr->v[1] + eyePoint[1], (pm->am).Tr->v[2] + eyePoint[2],
	  (pm->am).Tr->v[0], (pm->am).Tr->v[1], (pm->am).Tr->v[2],
	    upVec[0],upVec[1],upVec[2]);
  else if (viewMode == 1) {
	  Vector3 vel = pm->VelLinear.scale(40.0/ pm->VelLinear.Length());

	  gluLookAt((pm->am).Tr->v[0] + vel.v[0] + 20, (pm->am).Tr->v[1] + vel.v[1], (pm->am).Tr->v[2] + vel.v[2] + eyePoint[2],
		  (pm->am).Tr->v[0], (pm->am).Tr->v[1], (pm->am).Tr->v[2],
		  upVec[0], upVec[1], upVec[2]);
  }
  else if (viewMode == 2) {
	  Vector3 vel = pm->VelLinear.scale(40.0 / pm->VelLinear.Length());

	  gluLookAt((pm->am).Tr->v[0] - vel.v[0] + eyePoint[0], (pm->am).Tr->v[1] - vel.v[1] + eyePoint[1], (pm->am).Tr->v[2] - vel.v[2],
		  (pm->am).Tr->v[0] + vel.v[0], (pm->am).Tr->v[1] - vel.v[1], (pm->am).Tr->v[2] - vel.v[2],
		  -1, 0, upVec[2]);
  }
}

static void setProjection3D()
{
  GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (float)WinWidth / (float)WinHeight;
  gluPerspective(fov, aspect, nearPlane, farPlane);
  /* Back to normal */
  glMatrixMode(GL_MODELVIEW);
  seteyePoint();

  if (hiddenDepth)
  {
	// Now do z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthRange(nearPlane,farPlane);
	glClearDepth(farPlane);
  }
  else
  {
	glDisable(GL_DEPTH_TEST);
  }

  if (hiddenCull)
  {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  if (clockWise) glFrontFace(GL_CW);
	  else glFrontFace(GL_CCW);
  }
  else
  {
	  glDisable(GL_CULL_FACE);
  }
}

static void setProjection2D()
{
  //GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //aspect = (float)WinWidth / (float)WinHeight;
  glOrtho(0, WinWidth, 0, WinHeight, -1.0f, 1.0f);
  /* Back to normal but identity*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
}

void drawCube()
{
	glPushMatrix();
	cube->render();
	glPopMatrix();
}

void drawCylinder()
{	
	glPushMatrix();
	gluCylinder(quadric1,0.5,0.5,1,17,4);
	glPopMatrix();
}

static void drawWorld ()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}
  glPushMatrix();
  
  if (dofog)
  {
	  //printf("foggy\n");
	  glEnable(GL_FOG);
	  glFogi(GL_FOG_MODE, GL_LINEAR); // try GL_EXP
									  //glFogi(GL_FOG_MODE, GL_EXP2); // try GL_EXP
	  glFogfv(GL_FOG_COLOR, fogColour);
	  glFogf(GL_FOG_DENSITY, 0.004);   // doesn't affect GL_LINEAR fog
	  glFogf(GL_FOG_START, 40);    // doesn't affect GL_EXP fog
	  glFogf(GL_FOG_END, skySphereRad*1.0); 	   // doesn't affect GL_EXP fog
  }
  else
  {
	  glDisable(GL_FOG);
  }

	  seteyePoint();
	  pm->render();
	  glPushMatrix();
	  glTranslatef((pm->am).Tr->v[0], (pm->am).Tr->v[1] - skySphereRad, (pm->am).Tr->v[2]);
	  SetDirt();
	  skySphere->renderWithNormalsIn();
	  glPopMatrix();

	  if (showGrid)
	  {
		  SetDirt();
		  SetAl();
		  //SetYellowSand();
		  //glColor3f(0.5, 0.5, 0);
		  glPushMatrix();
		  glTranslatef(50, 0, 50);
		  grid->render();
		  glPopMatrix();
	  }

	  glPushMatrix();
	  float scale = 5;
	  glTranslatef(100, 0.02, 100 - scale*1.5);
	  glScalef(scale, scale, scale);
	  glRotatef(90, 0, 0, 1);
	  glTranslatef(0, -1.5,0);
	  SetAl();
	  //if(pm->IsStarted)

	  tex_q->render1(true);
	  glPopMatrix();

	  renderHab(100, 60);
	  renderHab(80, 60);
	  renderHab(70, 100);

	  for (int i = 70; i < 110; i += 8) {
		  renderSolar(i);
	  }
	  
	  glRotatef(180, 0, 1, 0);
	  glTranslatef(terrX, terrY, terrZ);
	  terr->render(terrX, terrY, terrZ, (pm->am).Tr->v[0], (pm->am).Tr->v[1] - skySphereRad, (pm->am).Tr->v[2]);

	  glPopMatrix();


	  //terr->render(0, 0, 0, (pm->am).Tr->v[0], (pm->am).Tr->v[1] - skySphereRad, (pm->am).Tr->v[2]);
	  //if (showBase1)
	  //{
		 // drawBase1();
	  //}
	  //if (showBase2)
	  //{
		 // drawBase2();
	  //}
	  //if (showSpin) glPopMatrix();
 

  glPushMatrix();
  pm->ApplyAffine();
  glTranslatef(-0.5, -6, 1.75);
  glRotatef(180, 0, 0, 1);
  glRotatef(135, 0, 1, 0);
  for (int jjj = 0; jjj < 10; jjj++) {
	  SetWhite();
	  drawPsystem(jjj);
  }
  glPopMatrix();

}

void draw2DStuff()
{
	glColor3f(1,1,0);
	glRasterPos2f(30,60);
	
	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	//drawText2d->drawstr(30, 30, "%s (%d)",dispString,45);

	drawText2d->drawstr(10, 530, "%s %8.2f,%8.2f,%8.2f, ","EyePoint",eyePoint[0],eyePoint[1],eyePoint[2]);

	drawText2d->drawstr(10, 500, "%s","2 - 2D mode and help");
    drawText2d->drawstr(10, 470, "%s","3 - 3D mode");
    drawText2d->drawstr(10, 440, "%s","5 - 2D & 3D mode");
    drawText2d->drawstr(10, 410, "%s","g - grid");
    drawText2d->drawstr(10, 380, "%s","a - axes");
    drawText2d->drawstr(10, 350, "%s","w - Wireframe");
    drawText2d->drawstr(10, 320, "%s","o - Space Base one");
    drawText2d->drawstr(10, 290, "%s","t - space Base two");
    drawText2d->drawstr(10, 260, "%s","F1 - This help");
    drawText2d->drawstr(10, 230, "%s","Arrows and pgup/pgdown comma and dot move eyepos");
    drawText2d->drawstr(10, 200, "%s","Space = start / stop spin");
    drawText2d->drawstr(10, 170, "%s","F2 - restore start view");

	drawText2d->drawstr(10, 150, "%s","d - depth buffer");
    drawText2d->drawstr(10, 130, "%s","c - cull hidden  @-swaps CW/CCW");
    drawText2d->drawstr(10, 110, "%s","/ - spin speed");
    drawText2d->drawstr(10, 90 , "%s","+,- and = sets angle variable");

  }

static void display()
{
  if (hiddenDepth) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  else {glClear(GL_COLOR_BUFFER_BIT);}
  setUpLights();
  if (mode == 3 || mode == 5)
	{
		setProjection3D();
		seteyePoint();
		drawWorld();
	}

  if (mode == 2 || mode == 5)
	{
		setProjection2D();
		draw2DStuff();
	}
  /* Check everything OK and update screen */
  CheckGL();
  glutSwapBuffers();
}

static void resize (int width, int height)
{
  /* Save for event handlers */
  WinWidth  = width;
  WinHeight = height;
  
  /* Reset view in window. */
  glViewport(0, 0, WinWidth, WinHeight);
}

void readTextureFile()
{
	bmp0 = new CBitmap("C:\\Users\\TomDa\\Desktop\\images.bmp");
	printf("bmp size %d %d\n", bmp0->GetWidth(), bmp0->GetHeight());

	glGenTextures(1, &texHandle);

	// Texture handle will be bound to the BMP bitmap data just
	// as it was read in...
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexImage2D(GL_TEXTURE_2D,
		0,                              // Mipmap level 0
		3,                              // 3 bytes [R,G,B] per pixel
		bmp0->GetWidth(),   // width of bitmap
		bmp0->GetHeight(),  // height of bitmap
		0,                              // no border pixels
		GL_RGBA,                     // bitmap pixel format
		GL_UNSIGNED_BYTE,               // format for GL to store it
		bmp0->GetBits()               // pointer to the pixel data
	);
	//glBindTexture(GL_TEXTURE_2D, texHandle);
}
/****		User events		****/


static void menuChoice (int item)
{
  switch (item) {
    case cmdRed:
        color3[0]=1; // R
        color3[1]=0; // G
        color3[2]=0; // B
      break;
    case cmdGreen:
        color3[0]=0; // R
        color3[1]=1; // G
        color3[2]=0; // B
      break;
    case cmdExit:
      exit(0);
      break;
    default:
      break;
  }
}
void initPsystemToDefault(PsystemStruct *ps, int num)
{
	ps->active = false;
	ps->activeticks = -1;
	ps->typeOfPsystem = 0;       // future expansion
	ps->texHandle = 0;           // sane value
	ps->billboard = false;       // sane value
	v3FloatToVecM(ps->pos, num % 4, 0, num / 4);
	ps->emissionRate = 1;      // number of particles per tick (can be fractional)
	ps->emmisionRateRandom = 0;  // a max random number which we div by 100 and add to emision rate
	ps->residualEmmision = 0;
	ps->maxParticles = 100;      // no more than this please
	ps->numParticles = 0;        // the actual number 
	ps->particleSize = 3;        // size in points 
	ps->particleSizeDelta = 0;   // no change
	ps->max_ticks = 120;         // after this many ticks die
	v3FloatToVec(-10000, 0, -10000, ps->dieIfLess);      // kill the particl if its less than any of these values
	v4FloatToVecM(ps->colour, 0.1, 0.8, 0.1, 1); // particle color       
	v4FloatToVecM(ps->colourChange, 0, 0, 0, 0);   // colour drift + added each tick
	v4FloatToVecM(ps->colourMax, 1, 1, 1, 1);
	v3FloatToVec(0, -0.02, 0, ps->gravity);    // velosity change due to gravity
	v3FloatToVec(0, 0, 0, ps->wind);           // movement due to wind
	v3FloatToVec(-0.025, 0.3, -0.025, ps->initialVelosity);
	v3FloatToVec(75, 45, 75, ps->initialVelosityRandom); // max random number which we dive by 100 and add to initial velosity 
	v3FloatToVec(0, 0, 0, ps->airResistance);       // how it slows
	v3FloatToVec(5, 5, 5, ps->absTerminalVelosity);    // max speed

	for (int i = 0; i< MAX_PSYSTEM; i++)
	{
		ps->p[i].active = false;   // active Y or n
		ps->p[i].time = 0;      // ticks of activity
		v3FloatToVec(0, 0, 0, ps->p[i].pos);      // position
		v3FloatToVec(0, 0, 0, ps->p[i].velosity); // how fast do i move
	}
}

void initPsystems()
{
	for (int num = 0; num<10; num++) initPsystemToDefault(&(psa[num]), num);
}
void initPsystem0()
{
	//psa[0].p->pos.x = pm->am.Tr->v[0];
	//psa[0].p->pos.y = pm->am.Tr->v[1];
	//psa[0].p->pos.z = pm->am.Tr->v[2];
}
void initPsystem1()
{
	psa[1].maxParticles = 10;
}

void initPsystem2()
{
	psa[2].typeOfPsystem = 1;
}

void initPsystem3()
{
	psa[3].typeOfPsystem = 1;
	psa[3].activeticks = 3;
	psa[3].emissionRate = 100;
	psa[3].maxParticles = 300;
	psa[3].emmisionRateRandom = 100;
}

void initPsystem4()
{
	psa[4].typeOfPsystem = 2;
	psa[4].activeticks = 30000;
	psa[4].emissionRate = 0.2;
	psa[4].maxParticles = 10;
	psa[4].emmisionRateRandom = 10;
	psa[4].particleSize = 0.5;         // size in points 
	psa[4].particleSizeDelta = 0.03;   // size change
	psa[4].texHandle = 1;
	psa[4].billboard = true;
}

void initPsystem5()
{
	psa[5].typeOfPsystem = 2;
	psa[5].activeticks = 30000;
	psa[5].emissionRate = 0.2;
	psa[5].maxParticles = 10;
	psa[5].emmisionRateRandom = 10;
	psa[5].particleSize = 0.5;         // size in points 
	psa[5].particleSizeDelta = 0.03;   // size change
	psa[5].texHandle = 1;
	psa[5].billboard = false;
}

void initPsystem6()
{
	psa[6].typeOfPsystem = 2;
	psa[6].activeticks = 30000;
	psa[6].emissionRate = 1;
	psa[6].maxParticles = 300;
	psa[6].emmisionRateRandom = 10;
	psa[6].particleSize = 1;         // size in points 
	psa[6].particleSizeDelta = 0.5;   // size change
	psa[6].texHandle = 1;
	psa[6].billboard = false;
	psa[6].initialVelosity = vec3(0, 0.4, 0);
	psa[6].gravity = vec3(0, 0, 0);
	psa[6].wind = vec3(0.0, 0.8, 0);

}

void loadTextures()
{
	GLubyte r, g, b;

	glGenTextures(10, ttex);

	texTemp = new CBitmap(findFile("BH.bmp"));
	if (texTemp->GetWidth() == 0)
	{
		printf("ERROR with bitmap\n");
		exit(1);
	}


	// Texture handle 0 will be bound to the BMP bitmap data just
	// as it was read in...
	glBindTexture(GL_TEXTURE_2D, ttex[0]);

	//for(int i=0;i<128; i++)
	//  printf("char %d\n",(int)texTemp.BitmapBits[i]);

	glTexImage2D(GL_TEXTURE_2D,
		0,                              // Mipmap level 0
		3,                              // 3 bytes [R,G,B] per pixel
		texTemp->GetWidth(),   // width of bitmap
		texTemp->GetHeight(),  // height of bitmap
		0,                              // no border pixels
		GL_BGR_EXT,                     // bitmap pixel format
		GL_UNSIGNED_BYTE,               // format for GL to store it
		texTemp->GetBits()              // pointer to the pixel data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Now add an alpha channel to the bitmap...

	GLubyte *bitz = (GLubyte *)texTemp->GetBits(); // set adress for future use

	for (int x = 0; x<128; x++)
		for (int y = 0; y<128; y++)
		{
			// Copy the r,g,b components across to tex1 -- reorder them from
			// BMP's eccentric BGR sequence to more usual RGB...
			r = bitz[(y * 128 + x) * 4];
			g = bitz[(y * 128 + x) * 4 + 1];
			b = bitz[(y * 128 + x) * 4 + 2];
			tex1[y][x][0] = r;
			tex1[y][x][1] = g;
			tex1[y][x][2] = b;
			// Add an alpha channel such that one colour becomes transparent
			// while any other colour remains opaque...
			if (tex1[y][x][0] == 255      // R
				&& tex1[y][x][1] ==255   // G
				&& tex1[y][x][2] == 255) // B
			{
				tex1[y][x][3] = 0; // alpha = 0  (transparent)
			}
			else
			{
				tex1[y][x][3] = 255 - tex1[y][x][0]; // alpha = 255 (opaque)
			}
		}
	// Attach the pixel data to texture handle 1...
	glBindTexture(GL_TEXTURE_2D, ttex[1]);
	glTexImage2D(GL_TEXTURE_2D,
		0,          // Mipmap level 0
		4,          // 4 bytes per pixel (RGBA)
		128,         // bitmap width
		128,         // bitmap height
		0,          // No border pixels
		GL_RGBA,    // Order of pixel data in memory
		GL_UNSIGNED_BYTE,  // How GL to store 
		tex1        // Pointer to pixel data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey (unsigned char key, int x, int y)
{
  if (key == ESC)
    menuChoice(cmdExit);

  if (key =='2') {mode = 2; strcpy(dispString,"2D Mode");}
  if (key =='3') {mode = 3; strcpy(dispString,"3D Mode");}
  if (key =='5') {mode = 5; strcpy(dispString,"2D+3D Mode");}
  if (key =='g') {showGrid=!showGrid;}
  if (key =='a') { showAxes=!showAxes;}
  if (key =='o') { showBase1=!showBase1;}
  if (key =='t') { showBase2=!showBase2;}
  if (key =='w') { showWireFrame=!showWireFrame;}
  if (key ==' ') { showSpin=!showSpin;}

  if (key =='d') {hiddenDepth=!hiddenDepth;}
  if (key =='c') {hiddenCull=!hiddenCull;}
  if (key =='@') {clockWise=!clockWise;}
  if (key =='/') {spinIncrement=spinIncrement+1; if (spinIncrement > 4) {spinIncrement=0.4f;}}
  //if (key =='j') {showChrisTrapani=!showChrisTrapani;}

  if (key =='=') {if (angle < 50) angle = 90; else angle = 0;}
  if (key =='+') {angle = angle + 10; angle = (GLfloat)((int)angle % 360);}
  if (key =='-') {angle = angle - 10; angle = (GLfloat)((int)angle % 360);}

  if (key =='.') {eyePoint[0]=eyePoint[0]+2;eyePoint[2]=eyePoint[2]+2;}
  if (key ==',') {eyePoint[0]=eyePoint[0]-2;eyePoint[2]=eyePoint[2]-2;}


  if (key == 'j') { rocs[0]->IsOn = true; rocs[2]->IsOn = true;}
  if (key == 'l') { rocs[1]->IsOn = true; rocs[3]->IsOn = true; }
  if (key == 'i') { rocs[5]->IsOn = true; rocs[7]->IsOn = true; }
  if (key == 'k') { rocs[4]->IsOn = true; rocs[6]->IsOn = true; }

  if (key == 'z') { rocs[8]->IsOn = true; pm->IsStarted = true; psa[6].active = true; psa[6].activeticks = 5000;}
}

static void asciiKey_up(unsigned char key, int x, int y)
{
	if (key == 'j') { rocs[0]->IsOn = false; rocs[2]->IsOn = false; }
	if (key == 'l') { rocs[1]->IsOn = false; rocs[3]->IsOn = false; }
	if (key == 'i') { rocs[5]->IsOn = false; rocs[7]->IsOn = false; }
	if (key == 'k') { rocs[4]->IsOn = false; rocs[6]->IsOn = false; }

	if (key == 'z') { rocs[8]->IsOn = false; psa[6].active = false;	}
}

void setStartValues()
{
  eyePoint[0] = 22.0;
  eyePoint[1] = 12.0;
  eyePoint[2] = 18.0;
  eyePos = vec3(22, 12, 18);
  lookAtPoint[0]=0;
  lookAtPoint[1]=0;
  lookAtPoint[2]=0;

  upVec[0]=0;
  upVec[1]=1;
  upVec[2]=0;

  fov=60; // field of view
  nearPlane=0.5;
  farPlane=900;

  spinAngle = 0.0;
}

static void specialKey (int key, int x, int y)
{
  
  if (key==GLUT_KEY_F1) {mode = 2;}

  if (key==GLUT_KEY_F2)
  {
  setStartValues();
  }

  if (key==GLUT_KEY_LEFT) eyePoint[0]=eyePoint[0]+2;
  if (key==GLUT_KEY_RIGHT) eyePoint[0]=eyePoint[0]-2;
  if (key==GLUT_KEY_UP) eyePoint[2]=eyePoint[2]-2;
  if (key==GLUT_KEY_DOWN) eyePoint[2]=eyePoint[2]+2;
  if (key==GLUT_KEY_PAGE_UP) eyePoint[1]=eyePoint[1]+2;
  if (key==GLUT_KEY_PAGE_DOWN) eyePoint[1]=eyePoint[1]-2;
}

void exitFunction(void)
{
  delete cube;
  delete axes;
  delete drawText2d; 
  delete grid;
  delete dome0;
  delete dome1;
  delete dome2;

  gluDeleteQuadric(quadric1);
  gluDeleteQuadric(quadric2);
  gluDeleteQuadric(quadric3);
  gluDeleteQuadric(quadric4);
//  gluDeleteQuadric(quadric2);
}

/****		Startup			****/

void BuildLander() {
	fp radius = 3;
	fp halfHeight = 4;

	pm = new PhysModel(100, halfHeight + 3, 100, (int)framesPerSecond, radius, halfHeight*2);
	//*(pm->am.M) = pm->am.M->RotByX(M_PI / 2);
	pm->dome = DomeClass(20, 18, 5, .6);
	halfHeight = 0.9*halfHeight;
//	rocket = new SmallRocket(Vector3(0, 3, 0), 1);
	int numOfRocs = 9;
	int numOfComs = 9;
	AbstractComponent** as = new AbstractComponent*[numOfComs];
	rocs = (new Rocket*[numOfRocs]);
	
	Rocket *r0 = new Rocket(Vector3(radius, -halfHeight, 0),'Z',3*M_PI / 2, 10,lightRocS1);
	as[0] = r0;
	rocs[0] = r0;
	Rocket *r1 = new Rocket(Vector3(radius, halfHeight, 0), 'Z', 3 * M_PI / 2, 10, lightRocS2);
	as[1] = r1;
	rocs[1] = r1;
	Rocket *r2 = new Rocket(Vector3(-radius, halfHeight, 0), 'Z', M_PI / 2, 10, lightRocS1);
	as[2] = r2;
	rocs[2] = r2;
	Rocket *r3 = new Rocket(Vector3(-radius, -halfHeight, 0), 'Z', M_PI / 2, 10, lightRocS2);
	as[3] = r3;
	rocs[3] = r3;

	Rocket *r4 = new Rocket(Vector3(0, -halfHeight, radius), 'X',  M_PI / 2, 10, lightRocS1);
	as[4] = r4;
	rocs[4] = r4;
	Rocket *r5 = new Rocket(Vector3(0, halfHeight, radius), 'X',  M_PI / 2, 10, lightRocS2);
	as[5] = r5;
	rocs[5] = r5;
	Rocket *r6 = new Rocket(Vector3(0, halfHeight, -radius), 'X', 3 * M_PI / 2, 10, lightRocS1);
	as[6] = r6;
	rocs[6] = r6;
	Rocket *r7 = new Rocket(Vector3(0, -halfHeight, -radius), 'X', 3 * M_PI / 2, 10, lightRocS2);
	as[7] = r7;
	rocs[7] = r7;

	Rocket *rB = new Rocket(Vector3(0, -halfHeight,0), 'X', M_PI, 10, lightRocB);
	rB->IsBig = true;
	rB->force = 30;
	as[8] = rB;
	rocs[8] = rB;

	sp2 = new SolarPanel2(0, 'x', 0, 0, 0, framesPerSecond, 1);
	sp1 = new SolarPanel2(0, 'x', 0, 0, 0, framesPerSecond, 1);


	pm->setComponents(as, numOfComs);
	pm->setRockets(rocs, numOfRocs);

	solar1 = new SolarPanel1(360, 'y', 0, 0, 0, framesPerSecond * 5, 12);
}

static void initGraphics(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* colour green */
	color3[0] = 0;
	color3[0] = 1;
	color3[0] = 0;

	/* Needed for vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Popup menu attached to right mouse button */
	AppMenu = glutCreateMenu(menuChoice);
	glutSetMenu(AppMenu);
	glutAddMenuEntry("Red", cmdRed);
	glutAddMenuEntry("Green", cmdGreen);
	glutAddMenuEntry("----", 0);
	glutAddMenuEntry("Exit", cmdExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Start values */
	spinAngle = 0.0;

	eyePoint[0] = 2.0;
	eyePoint[1] = 2.0;
	eyePoint[2] = 4.0;

	lookAtPoint[0] = 0;
	lookAtPoint[1] = 0;
	lookAtPoint[2] = 0;

	upVec[0] = 0;
	upVec[1] = 1;
	upVec[2] = 0;

	fov = 60; // field of view
	nearPlane = 0.5;
	farPlane = 80;

	menuChoice(cmdGreen);
	setStartValues();
	cube = new CubeClass();
	axes = new Axes();

	lightSun = new lightClass();
	lightRocB = new lightClass();
	lightRocS1 = new lightClass();
	lightRocS2 = new lightClass();
	lightSpot = new lightClass();

	readTextureFile();
	//bmp0= new CBitmap("C:\\VIC2011_TextureTute_V3\\MT2\\Cube1\\smile.bmp");
	//printf("bmp size %d %d\n",bmp0->GetWidth(),bmp0->GetHeight());
	terr = new terrain;
	terr->setPts();
	terr->setNormals();
	tex_q = new tex_quad();
	tex_q->setTextureHandle(texHandle);
	tex_t = new tex_triangle();

	//lightSun->setKind(lightClass::spotL);
	lightSun->setGlobalAmbient(0.9, 0.9, 0.9, 1);

	lightSun->setSpotDir(0, 0, -1);
	lightSun->setLightId(GL_LIGHT0);
	lightSun->setLightId(GL_LIGHT0);
	lightSun->setAmbient(0.30, 0.3, 0.3, 1); // just a bit of ambient white
	lightSun->setDiffuse(0.6, 0.6, 0.6f, 1); // fair bit of diffuse white
	lightSun->setSpecular(0.4, 0.4, 0.3f, 1); //fair bit of spec white 
	lightSun->setPos(0, 1, -2.5, 0);

	lightRocB->setSpotDir(0, 0, -1);
	lightRocB->setLightId(GL_LIGHT1);
	lightRocB->setAmbient(0.9, 0.1, 0.1, 1); // just a bit of ambient white
	lightRocB->setDiffuse(0.9f, 0.1f, 0.1f, 1); // fair bit of diffuse blue
	lightRocB->setSpecular(0.9f, 0.1f, 0.1f, 1); //fair bit of spec white 
	lightRocB->setPos(0, 0, 0, 1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.03);

	lightRocS1->setSpotDir(0, 0, -1);
	lightRocS1->setLightId(GL_LIGHT2);
	lightRocS1->setAmbient(1, 1, 1, 1); // just a bit of ambient white
	lightRocS1->setDiffuse(0, 0.2, 1, 1); // fair bit of diffuse yellow
	lightRocS1->setSpecular(0, 0.2, 1, 1); //fair bit of spec yellow 
	lightRocS1->setPos(0, 0, 0, 1);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.03);

	lightRocS2->setSpotDir(0, 0, -1);
	lightRocS2->setLightId(GL_LIGHT3);
	lightRocS2->setAmbient(1, 1, 1, 1); // just a bit of ambient white
	lightRocS2->setDiffuse(0, 0.2, 1, 1); // fair bit of diffuse yellow
	lightRocS2->setSpecular(0, 0.2, 1, 1); //fair bit of spec yellow 
	lightRocS2->setPos(0, 0, 0, 1);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.03);
	

	lightSpot->setKind(lightClass::lightKind::spotL);
	lightSpot->setSpotDir(0, 0, -1);
	lightSpot->setLightId(GL_LIGHT4);
	lightSpot->setAmbient(0.0, 0.0, 0.0, 1); // just a bit of ambient white
	lightSpot->setDiffuse(0.05, 0.9, 0.05f, 1); // fair bit of diffuse red
	lightSpot->setSpecular(0.05, 0.9, 0.05f, 1); //fair bit of spec red 
	lightSpot->setPos(0, 0, 0, 1);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.009);
	
	cube = new CubeClass();
	dome0 = new DomeClass();
	dome1 = new DomeClass(19, 10, 9, 4);
	dome2 = new DomeClass(17, 12, 2, 5);
	dome2->startAngle = 10;
	domeHab = new DomeClass(17, 20, 5, 5);
	axes = new Axes();
	drawText2d = new Text2D();
	grid = new Grid();
	grid->setUp(60, 60, 20, 20);
	quadric1 = gluNewQuadric();
	quadric2 = gluNewQuadric();
	quadric3 = gluNewQuadric();
	quadric4 = gluNewQuadric();

	BuildLander();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	spinAngle = 0.0;
	initPsystems();
	initPsystem0();
	initPsystem1();
	initPsystem2();
	initPsystem3();
	initPsystem4();
	initPsystem5();
	initPsystem6();
	eyePos.x = 6;
	eyePos.y = 4;
	eyePos.z = 6;

	//psa[6].active = true;
	//psa[6].activeticks = 100;
	//psa[0].active = true;
	//psa[0].activeticks = 100000;

	atexit(exitFunction);
}

static void initGraphics2 (void)
{
  /* Black background */
  glClearColor(0, 0, 0, 0);

  /* colour green */
  color3[0]=0;
  color3[0]=1;
  color3[0]=0;

  glEnable(GL_DEPTH_TEST);

  /* Needed for vertex arrays */
  glEnableClientState(GL_VERTEX_ARRAY);
  
  /* Popup menu attached to right mouse button */
  AppMenu = glutCreateMenu(menuChoice);
  glutSetMenu(AppMenu);
  glutAddMenuEntry("Red", cmdRed);
  glutAddMenuEntry("Green", cmdGreen);
  glutAddMenuEntry("----", 0);
  glutAddMenuEntry("Exit", cmdExit);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* Start values */
   setStartValues();

  menuChoice(cmdGreen);

  cube = new CubeClass();
  dome0 = new DomeClass();
  dome1 = new DomeClass(19,10,9,4);
  dome2 = new DomeClass(17,12,2,5);
  dome2->startAngle = 10;
  domeHab = new DomeClass(17, 20, 5, 5);
  axes = new Axes();
  drawText2d = new Text2D();
  grid = new Grid();
  quadric1=gluNewQuadric();
  quadric2=gluNewQuadric();
  quadric3=gluNewQuadric();
  quadric4=gluNewQuadric();
  BuildLander();
  atexit (exitFunction);
}

void timer(int value)
{
	timeForNextFrame = true;
	glutTimerFunc(timerInterval, timer, 0);
}
/****		Main control		****/


static void idleRoutine(void)
{
  if (timeForNextFrame)
  {
	  spinAngle += spinIncrement;
	if (spinAngle > 360.0)
			spinAngle -= 360.0;


	if (angleVar < angle )
	{
		angleVar = angleVar+1;
		if (angleVar > angle ) angleVar = angle;
	}
	for (int num = 0; num < 10; num++)
	{
		psSpinAngle = spinAngle;
		setPsEyePos(eyePos);
		updatePsystem(&(psa[num]));
	}

	glutPostRedisplay();
	timeForNextFrame = false;
  }
}
int main (int argc, char * argv[])
{
	srand((unsigned)time(NULL)); // microsoft
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize(550, 550);
  glutInitWindowPosition(100, 75);
  glutCreateWindow("Cube");

  initGraphics();
  loadTextures();
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutKeyboardFunc(asciiKey);
  glutKeyboardUpFunc(asciiKey_up);
  glutSpecialFunc(specialKey);
  
  glutIdleFunc(idleRoutine);
  glutTimerFunc(timerInterval, timer, 0);
  
  glutMainLoop();
  /* Should never get here, but keeps compiler happy */
  return 0;
}
