/****		Dome form	****/

#include "Dome.h"

#include <math.h>

void Move2D(GLfloat oldx, GLfloat oldz,
	        GLfloat angle, GLfloat distance, // angle in degrees
	        GLfloat *resultx, GLfloat *resultz)
{
	GLfloat tmpx, tmpz;
	GLfloat newx, newz;
	float yaw; // yaw is an angle in radians

	yaw = angle*3.1416 / 180;


	tmpx = 0;/* set to origin */
	tmpz = distance; /* move us forward by speed */

	newx = (tmpz * sin(yaw)) + (tmpx * cos(yaw));
	newz = (tmpz * cos(yaw)) - (tmpx * sin(yaw));
	tmpx = newx;
	tmpz = newz;

	newx = tmpx + oldx; // adjust because it was around the origin
	newz = tmpz + oldz;

	*resultx = newx;
	*resultz = newz;
}

DomeClass::DomeClass(int slicez, int stackz, GLfloat deltaz, GLfloat radiuz) // constructor
{
//	GLfloat x0, y0, z0;
//	GLfloat x1, y1, z1;
//	GLfloat x2, y2, z2;
//	GLfloat x3, y3, z3;

	slices = slicez;
	stacks = stackz;
	//clipHeight = clipHeightz; 
	delta = deltaz; // angle in degrees
	radius = radiuz;
	debug = false;

}

DomeClass::DomeClass() // constructor
{
	// just a shell all importanyt initialisation is done already 
}

DomeClass::DomeClass(DomeClass *d)// copy constructor
{
	slices = d->slices;
	stacks = d->stacks;
	startAngle = d->startAngle;
	delta = d->delta; // angle in degrees
	radius = d->radius;
	debug = d->debug;
}

void DomeClass::renderWithoutNormals() // draw it
{
	int sl, st;

	GLfloat theta = 0;
	GLfloat fi = startAngle;
	GLfloat dtheta = 360.0 / slices;
	GLfloat dfi = delta;
	GLfloat pfi;

	//GLfloat nx, ny, nz;
	//GLfloat nx2, ny2, nz2;
	GLfloat xx, yy;
	GLfloat ptheta;
	GLfloat pw, w;
	GLfloat ph, h;

	h = 0;
	ph = 0;
	w = 0;
	pw = 0;

	//stacks = 2;
	for (st = 0; st < stacks; st++)
	{
		pfi = fi;
		fi = fi + dfi;
        pw = w;
        ph = h;

		Move2D(0, 0, fi, radius, &xx, &yy); // angle in degrees
		h = radius - yy;
		w = xx;
		if (debug) printf(">> %f , %f \n", h, w);
		for (sl = 0;sl < slices;sl++)
		{
			ptheta = theta;
			theta = theta + dtheta;
			
			if (st == 0)
			{
				if (startAngle == 0)
				{
					x0 = 0;
					y0 = 0;
					z0 = 0;

					//h = -1;
					//w = 3;

					y1 = h;
					y2 = h;
					Move2D(0, 0, ptheta, w, &x1, &z1); // angle in degrees
					Move2D(0, 0, theta, w, &x2, &z2); // angle in degrees

					glBegin(GL_TRIANGLES);
					glVertex3f(x0, y0, z0);
					glVertex3f(x1, y1, z1);
					glVertex3f(x2, y2, z2);
					glEnd();
				}
			}
			else // stacks != 0
			{
				//h = -2;
				//w = 5;

				y0 = ph;
				y1 = ph;
				Move2D(0, 0, ptheta, pw, &x0, &z0); // angle in degrees
				Move2D(0, 0, theta, pw, &x1, &z1); // angle in degrees

				y2 = h;
				y3 = h;
				Move2D(0, 0, ptheta, w, &x2, &z2); // angle in degrees
				Move2D(0, 0, theta, w, &x3, &z3); // angle in degrees
				
				glBegin(GL_QUADS);
				glVertex3f(x2, y2, z2);				
				glVertex3f(x3, y3, z3);	
				glVertex3f(x1, y1, z1);
				glVertex3f(x0, y0, z0);
				glEnd();
			}
			
		}
		
	}
}

void DomeClass::renderWithNorm() // draw it
{
	int sl, st;

	GLfloat theta = 0;
	GLfloat fi = startAngle;
	GLfloat dtheta = 360.0 / slices;
	GLfloat dfi = delta;
	GLfloat pfi;

	//GLfloat nx, ny, nz;
	//GLfloat nx2, ny2, nz2;
	GLfloat xx, yy;
	GLfloat ptheta;
	GLfloat pw, w;
	GLfloat ph, h;

	h = 0;
	ph = 0;
	w = 0;
	pw = 0;

	//stacks = 2;
	for (st = 0; st < stacks; st++)
	{
		pfi = fi;
		fi = fi + dfi;
		pw = w;
		ph = h;

		Move2D(0, 0, fi, radius, &xx, &yy); // angle in degrees
		h = radius - yy;
		w = xx;
		if (debug) printf(">> %f , %f \n", h, w);
		for (sl = 0;sl < slices;sl++)
		{
			ptheta = theta;
			theta = theta + dtheta;

			if (st == 0)
			{
				if (startAngle == 0)
				{
					x0 = 0;
					y0 = 0;
					z0 = 0;

					//h = -1;
					//w = 3;

					y1 = h;
					y2 = h;
					Move2D(0, 0, ptheta, w, &x1, &z1); // angle in degrees
					Move2D(0, 0, theta, w, &x2, &z2); // angle in degrees

					norm0.x = x0;
					norm0.y = y0-radius;
					norm0.z = z0;
					norm1.x = x1;
					norm1.y = y1-radius;
					norm1.z = z1;
					norm2.x = x2;
					norm2.y = y2-radius;
					norm2.z = z2;

					if (facingNormals)
					{
					norm0 = norm0*(-1);
					norm1 = norm1*(-1);
					norm2 = norm2*(-1);
					}

					norm0 = normalize(norm0);
					norm1 = normalize(norm1);
					norm2 = normalize(norm2);

					glBegin(GL_TRIANGLES);
					glNormal3fv(norm0.fv());
					glVertex3f(x0, y0, z0);
					glNormal3fv(norm1.fv());
					glVertex3f(x1, y1, z1);
					glNormal3fv(norm2.fv());
					glVertex3f(x2, y2, z2);
					glEnd();

					if (drawNormals)
					{
						glBegin(GL_LINES);
						glVertex3f(x0, y0, z0); glVertex3f(x0 + norm0.x, y0 + norm0.y, z0 + norm0.z);
						glVertex3f(x1, y1, z1); glVertex3f(x1 + norm1.x, y1 + norm1.y, z1 + norm1.z);
						glVertex3f(x2, y2, z2); glVertex3f(x2 + norm2.x, y2 + norm2.y, z2 + norm2.z);
						glEnd();
					}
				}
			}
			else // stacks != 0
			{
				//h = -2;
				//w = 5;

				y0 = ph;
				y1 = ph;
				Move2D(0, 0, ptheta, pw, &x0, &z0); // angle in degrees
				Move2D(0, 0, theta, pw, &x1, &z1); // angle in degrees

				y2 = h;
				y3 = h;
				Move2D(0, 0, ptheta, w, &x2, &z2); // angle in degrees
				Move2D(0, 0, theta, w, &x3, &z3); // angle in degrees

				norm0.x = x0;
				norm0.y = y0 - radius;
				norm0.z = z0;
				norm1.x = x1;
				norm1.y = y1 - radius;
				norm1.z = z1;
				norm2.x = x2;
				norm2.y = y2 - radius;
				norm2.z = z2;
				norm3.x = x3;
				norm3.y = y3 - radius;
				norm3.z = z3;

				if (facingNormals)
				{
					norm0 = norm0*(-1);
					norm1 = norm1*(-1);
					norm2 = norm2*(-1);
					norm3 = norm3*(-1);
				}

				norm0 = normalize(norm0);
				norm1 = normalize(norm1);
				norm2 = normalize(norm2);				
				norm3 = normalize(norm3);

				glBegin(GL_QUADS);
				glNormal3fv(norm2.fv());
				glVertex3f(x2, y2, z2);
				glNormal3fv(norm3.fv());
				glVertex3f(x3, y3, z3);
				glNormal3fv(norm1.fv());
				glVertex3f(x1, y1, z1);
				glNormal3fv(norm0.fv());
				glVertex3f(x0, y0, z0);
				glEnd();

				if (drawNormals)
				{
					glBegin(GL_LINES);
					glVertex3f(x0, y0, z0); glVertex3f(x0 + norm0.x, y0 + norm0.y, z0 + norm0.z);
					glVertex3f(x1, y1, z1); glVertex3f(x1 + norm1.x, y1 + norm1.y, z1 + norm1.z);
					glVertex3f(x2, y2, z2); glVertex3f(x2 + norm2.x, y2 + norm2.y, z2 + norm2.z);					
					glVertex3f(x3, y3, z3); glVertex3f(x3 + norm3.x, y3 + norm3.y, z3 + norm3.z);
					glEnd();
				}
			}

		}

	}
}


void DomeClass::renderWithNormalsOut() // draw it
{
	facingNormals = false;
	drawNormals = false;
	renderWithNorm();
}

void DomeClass::renderWithNormalsIn()
{
	facingNormals = true;
	drawNormals = false;
	renderWithNorm();
}

void DomeClass::renderWithDrawnNormalsIn()
{
	facingNormals = true;
	drawNormals = true;
	renderWithNorm();
}

void DomeClass::renderWithDrawnNormalsOut()
{
	facingNormals = false;
	drawNormals = true;
	renderWithNorm();
}

void DomeClass::setdebug() // print co-ords to the console
{
	debug = true;
}

// end