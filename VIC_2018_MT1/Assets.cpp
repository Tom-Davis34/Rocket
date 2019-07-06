#include "Assets.h"

SolarPanel1::SolarPanel1(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime)
{
	Restart = true;
	StartChildrenOnStart = true;
	StartChildrenOnFinish = false;
	int numPanels = 12;
	SolarPanelSmall *last = new SolarPanelSmall(80, 'x', 0, 0, 0, frameRate, 3);
	addChild(last);
	for (int i = 1; i < numPanels; i++) {
		SolarPanelSmall *sp = new SolarPanelSmall(160, 'x', 0, 0, 0, frameRate, 3);
		last->addChild(sp);
		last = sp;
	}
	Start();
}

void SolarPanel1::renderSelf()
{
	glScalef(scaleFactor, scaleFactor, scaleFactor);

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glColor3f(0.4, 0.4, 0.4);
	dome->renderWithNormalsOut();
	glPopMatrix();

	glRotatef(-90, 1, 0, 0);

	//for (int i = 0; i < 10; i++) {
	//	
	//	glBegin(GL_QUADS);
	//	glColor3f(0, 0, 0.8);
	//	
	//	glVertex3f(-1,0,0);
	//	glVertex3f(-1, 0.90, 0);
	//	glVertex3f(1, 0.90, 0);
	//	glVertex3f(1, 0, 0);

	//	glColor3f(0.6, 0.6, 0.6);
	//	glVertex3f(-1, 0.90, 0);
	//	glVertex3f(-1, 1, 0);
	//	glVertex3f(1, 1, 0);
	//	glVertex3f(1, 0.90, 0);
	//	
	//	glEnd();
	//	glTranslatef(0, 1, 0);
	//}
}

void FuelPump::renderSelf()
{

}

void Hab1::renderSelf()
{
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
	dome->renderWithNormalsOut();
	glPopMatrix();

	//render legs
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
	gluCylinder(quadric1, 0.25, 0.25, 3, 20, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, radius * 0.8, height * 0.5);
	gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
	glPopMatrix();

	glPopMatrix();

}

SolarPanel2::SolarPanel2(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime) :OnRails(an, ax, translationX, translationY, translationZ, frameRate, RunningTime)
{
	int numPanels = 10;

	StartChildrenOnStart = true;
	StartChildrenOnFinish = false;

	Lid *lid = new Lid(100, 'x', 0, 0, 0, frameRate, 3);
	addChild(lid);

	Delay *delay = new Delay(frameRate, 3);
	addChild(delay);
	SolarPanelSmall *last = new SolarPanelSmall(80, 'x', 0, 0, 0, frameRate, 3);
	delay->addChild(last);
	for (int i = 1; i < numPanels; i++){
		SolarPanelSmall *sp = new SolarPanelSmall(160, 'x', 0, 0, 0, frameRate, 3);
		last->addChild(sp);
		last = sp;
	}
}

void SolarPanel2::renderSelf()
{
	//glScalef(scaleFactor, scaleFactor, scaleFactor);
	//glTranslatef(110, 1, 100);
	glMultMatrixf(am.getMap());

	glRotatef(-90, 1, 0, 0);

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0, 0.5, 0);
	glScalef(1.1, 1.1, 0.1);

	cube->render();
	glPopMatrix();
}

void SolarPanelSmall::renderSelf()
{
	glBegin(GL_QUADS);
	//glColor3f(0, 0, 0.8);
	SetBluePlas();
	glNormal3f(0,0,1);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.5, 0.90, 0);
	glVertex3f(0.5, 0.90, 0);
	glVertex3f(0.5, 0, 0);

	glColor3f(0.6, 0.6, 0.6);
	SetChrome();
	glNormal3f(0, 0, 1);
	glVertex3f(-0.5, 0.90, 0);
	glVertex3f(-0.5, 1, 0);
	glVertex3f(0.5, 1, 0);
	glVertex3f(0.5, 0.90, 0);

	glEnd();
	glTranslatef(0, 1, 0);
	glRotatef(-180, 0, 1, 0);
	glRotatef(-180, 1, 0, 0);
}

void Lid::renderSelf()
{
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0, 0.5, 0);
	glScalef(1.1, 1.1, 0.01);

	cube->render();
	glPopMatrix();
}

void Delay::renderSelf()
{
}

void SetBrass()
{
	float	dif1[] = { 0.78f, 0.57f, 0.11f, 1.0f }; // brass
	float	amb1[] = { 0.33f, 0.22f, 0.027f, 1.0f };
	float	spe1[] = { 0.99f, 0.94f, 0.8f, 1.0f };
	float   shiny1 = 0.218f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe1);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny1);
}

void SetPewter()
{
	float	dif2[] = { 0.43f, 0.47f, 0.54f, 1.0f }; // pewter
	float	amb2[] = { 0.1f, 0.06f, 0.11f, 1.0f };
	float	spe2[] = { 0.33f, 0.33f, 0.52f, 1.0f };
	float   shiny2 = 10.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe2);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny2);
}

void SetJade()
{
	float	dif3[] = { 0.54f, 0.89f, 0.63f, 1.0f }; // Jade
	float	amb3[] = { 0.135f, 0.22f, 0.15f, 1.0f };
	float	spe3[] = { 0.316f, 0.316f, 0.316f, 1.0f };
	float   shiny3 = 0.1f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe3);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny3);
}

void SetRedPlas()
{
	float	dif4[] = { 0.6f, 0.0f, 0.0f, 1.0f }; // red plastic - modified
	float	amb4[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float	spe4[] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float   shiny4 = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe4);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny4);
}

void SetOther()
{
	float	dif5[] = { 0.6f, 0.0f, 0.0f, 1.0f }; // Play With this one
	float	amb5[] = { 0.4f, 0.6f, 0.4f, 1.0f };
	float	spe5[] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float   shiny5 = 0.25f * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb5);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe5);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny5);
}

void SetChrome()
{
	//Chrome
	float mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	float mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
	float shine = 76.8f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

void SetWhite()
{
	float mat_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	float mat_diffuse[]= { 0.6f,0.6f,0.6f,1.0f };
	float mat_specular[] = { 0.6f,0.6f,0.9f,1.0f };
	float shine = 110.0f;

	//float mat_ambient[] = { 0.25f, 0.20725f, 0.20725f, 0.922f };
	//float mat_diffuse[] = { 1.0f, 0.829f, 0.829f, 0.922f };
	//float mat_specular[] = { 0.296648f, 0.296648f, 0.296648f, 0.922f };
	//float shine = 11.264f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

void SetDirt()
{
	//Perl
	float mat_ambient[] = { 0.3125f, 0.1275f, 0.054f, 1.0f };
	float mat_diffuse[] = { 0.814f, 0.4284f, 0.18144f, 1.0f };
	float mat_specular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
	float shine = 05.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void SetBluePlas()
{
//Cyan plastic
float mat_ambient[] = { 0.0f,0.1f,0.1f ,1.0f };
float mat_diffuse[] = { 0.0f,0.250980392f,0.60980392f,1.0f };
float mat_specular[] = { 0.50196078f,0.50196078f,0.50196078f,1.0f };
float shine = 32.0f;
glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

void SetBlueFlame()
{
	float mat_ambient[] = { 0.0f,0.1f,1 ,1.0f };
	float mat_diffuse[] = { 0.0f,0.250980392f,1.0f,1.0f };
	float mat_specular[] = { 0,0,0,1.0f };
	float shine = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void SetRedFlame()
{
	float mat_ambient[] = { 1,0.1f,0.0f ,1.0f };
	float mat_diffuse[] = { 0.92f,0.250980392f,0.0f,1.0f };
	float mat_specular[] = { 0,0,0,1.0f };
	float shine = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

void SetYellowSand()
{
	float mat_ambient[] = { 0.4,0.4f,0.1575f ,1.0f };
	float mat_diffuse[] = { 0.7, 0.7, 0.2,1.0f };
	float mat_specular[] = { 0.316228, 0.316228, 0.216228,1.0f };
	float shine = 0.1*128.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void SetAl()
{
	float mat_ambient[] = { 0.23125f, 0.23125f, 0.23125f, 1.0f };
	float mat_diffuse[] = { 0.2775f, 0.2775f, 0.2775f, 1.0f };
	float mat_specular[] = { 0.773911f, 0.773911f, 0.773911f, 1.0f };
	float shine = 89.6f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
