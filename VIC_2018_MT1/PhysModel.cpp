#include "PhysModel.h"


AbstractComponent::AbstractComponent(Vector3 displacementV, fp Moi, fp Mass)
{
	am = AffineMap(displacementV(0), displacementV(1), displacementV(2));
	moi = Moi;
	mass = Mass;
}

AbstractComponent::AbstractComponent(Vector3 displacementV, fp ang, char axis, fp Moi, fp Mass)
{
	am = AffineMap(displacementV(0), displacementV(1), displacementV(2),ang,axis);
	moi = Moi;
	mass = Mass;
}

void AbstractComponent::ApplyAffine()
{
	glMultMatrixf(am.getMap());
}

void PhysModel::update()
{
	am.Tr->v[0] += VelLinear(0);
	am.Tr->v[1] += VelLinear(1);
	am.Tr->v[2] += VelLinear(2);

	Matrix33 *m = new Matrix33();
	//		1,	-directionVect(2),directionVect(1),	
	//	directionVect(2),	1	,-directionVect(0),		
	//	-directionVect(1),directionVect(0),		1	,

	m->Matrix[0][1] = -VelAngular(2)*(1.0) / fRate;
	m->Matrix[0][2] = VelAngular(1)*(1.0) / fRate;
	m->Matrix[1][0] = VelAngular(2)*(1.0) / fRate;
	m->Matrix[1][2] = -VelAngular(0)*(1.0) / fRate;
	m->Matrix[2][0] = -VelAngular(1)*(1.0) / fRate;
	m->Matrix[2][1] = VelAngular(0)*(1.0) / fRate;

	Matrix33 *ptr = am.M;
	*am.M = (am.M->Multiply(*m));

	//am.M->scale(1.0/am.M->det());
}

void Rocket::render()
{
	glPushMatrix();
	ApplyAffine();

	if (!IsBig) {
		SetBrass();
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glColor3f(0.2, 0.2, 0.2);
		gluCylinder(gluNewQuadric(), 0.1, 0.3, 0.6, 17, 4);

		if (IsOn) {
			glTranslatef(0, 0, 0.6);
			glColor3f(0.7, 0, 0.2);
			SetBlueFlame();
			gluCylinder(gluNewQuadric(), 0.3, 0.0, 2, 17, 4);
			glTranslatef(0, 0, 0);

			light->letThereBeLight();
		}
		glPopMatrix();
	}
	else {
		SetBrass();
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glColor3f(0.2, 0.2, 0.2);
		gluCylinder(gluNewQuadric(), 0.4, 1.5, 1.5, 17, 4);

		if (IsOn) {
			SetRedFlame();
			glTranslatef(0, 0, 1.5);
			glColor3f(0.7, 0, 0.2);
			gluCylinder(gluNewQuadric(), 1.5, 0.0, 4, 17, 4);
			glTranslatef(0, 0, 0.5);
			light->letThereBeLight();
			if (force < 150) {
				force += 30 / 30;
			}
		}
		glPopMatrix();
	}

	glPopMatrix();
}

Vector3 Rocket::GetTorque()
{
	if (!IsOn)
		return Vector3();

	fp f = force * throttle;
	Vector3 forceVec = am.M->Multiply(Vector3(0, -1, 0), force);


	return forceVec.cross(*(am.Tr));
}

Vector3 Rocket::GetForce()
{
	if (!IsOn)
		return Vector3();

	fp f = force * throttle;
	Vector3 unitDir = (*(am.Tr)).scale(-1.0 / ((*(am.Tr)).Length()));
	Vector3 forceVec = am.M->Multiply(Vector3(0, 1, 0), force);
	return forceVec.scale(forceVec.dot(unitDir)/ forceVec.Length());
}

void PhysModel::setComponents(AbstractComponent **coms, int num)
{
	components = coms;
	numComs = num;
}

AbstractComponent* PhysModel::GetComponent(int num)
{
	return components[num];
}

void PhysModel::setRockets(Rocket ** rocs, int num)
{
	rockets = rocs;
	numRocs = num;
}

void PhysModel::updateVelocities()
{
	//iam = am.invert();

	Vector3 torq = Vector3();
	for (int i = 0; i < numRocs; i++) {
		Vector3 vec = rockets[i]->GetTorque();
		//vec = iam.rotate(vec);
		torq = torq.add(vec);
	}

	torq = torq.scale(1.0 / moi / fRate);
	
	VelAngular = VelAngular.add(torq);

	Vector3 forc = Vector3();
	for (int i = 0; i < numRocs; i++) {
		Vector3 vec = rockets[i]->GetForce();
		vec = am.rotate(vec);
		forc = forc.add(vec);
	}

	forc = forc.scale(1.0 / mass / fRate);

	VelLinear = VelLinear.add(forc);

	//accleration due to gravity
	VelLinear = VelLinear.add(Vector3(0, -1.63/10 / fRate, 0));
}

void PhysModel::ApplyAffine()
{
	glMultMatrixf(am.getMap());
}

void PhysModel::TurnRocketsOff()
{
	for (int i = 0; i < numRocs; i++) {
		rockets[i]->IsOn = false;
	}
}

void PhysModel::render()
{
	if (IsStarted) {
		update();
		updateVelocities();
	}
	
	glPushMatrix();
	ApplyAffine();
	SetWhite();
	//render cylinder
	glPushMatrix();
	glRotatef(90, 1,0,0);
	glColor3f(0.8, 0.8, 0.8);
	glTranslatef(0, 0, -height*0.5);
	glScalef(radius,  radius, height);
	gluCylinder(quadric1, 1, 1, 1, 20, 4);
	glPopMatrix();

	//render nose
	//glPushMatrix();
	//glRotatef(180, 1, 0, 0);
	//glTranslatef(0, -radius - height*0.5, 0);
	//dome.render();
	//glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 0.1, 0.4);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,0,  height * 0.5);
	gluCylinder(quadric1, radius, 0, radius*1.8, 20, 6);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 0.1, 0.4);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -height * 0.5);
	gluCylinder(quadric1, radius, 0, 0, 20, 6);
	glPopMatrix();

	//render legs
	//glPushMatrix();
	//glRotatef(90, 1, 0, 0);
	//glColor3f(0.1, 0.1, 0.1);

	//glPushMatrix();
	//glTranslatef(radius*0.8, 0, height * 0.5);
	//gluCylinder(quadric1, 0.25,  0.25, 3, 20, 4);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-radius*0.8, 0, height * 0.5);
	//gluCylinder(quadric1, 0.25, 0.25, 3, 20, 4);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(0, -radius * 0.8, height * 0.5);
	//gluCylinder(quadric1, 0.25, 0.25, 3, 20, 4);
	//glPopMatrix();

	float finSize = radius * 1.2;

	for (int i = 45; i < 360; i += 90) {

		glPushMatrix();
		SetBrass();
		glRotatef((float)i, 0, 1, 0);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_TRIANGLES);
		glVertex3f(0, -height * 0.5, radius);
		glVertex3f(0, -height * 0.5, radius + finSize);
		glVertex3f(0, -height * 0.5 + finSize, radius);
		glEnd();

		SetChrome();
		glPushMatrix();
		glColor3f(0.1, 0.1, 0.4);
		glRotatef(90, 1, 0, 0);
		float r = 0.6;
		glTranslatef(0, radius + r*0.5, 0);
		gluCylinder(quadric1, r, r, 6, 20, 6);
		glTranslatef(0, 0, -r);
		glRotatef(90, 1, 0, 0);
		dome.renderWithNormalsOut();
		glPopMatrix();

		glPopMatrix();
	}

	//rensder components
	for (int i = 0; i < numComs; i++) {
		GetComponent(i)->render();
	}
	glPopMatrix();

}


//void PhysModel::render()
//{
//	if (IsStarted) {
//		update();
//		updateVelocities();
//	}
//
//	glPushMatrix();
//	//glLoadIdentity();
//	ApplyAffine();
//
//	//render cylinder
//	glPushMatrix();
//	glRotatef(90, 1, 0, 0);
//	glColor3f(0.8, 0.8, 0.8);
//	glTranslatef(0, 0, -height * 0.5);
//	glScalef(radius, radius, height);
//	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 4);
//	glPopMatrix();
//
//	//render cylinder
//	glPushMatrix();
//	glRotatef(180, 1, 0, 0);
//	glTranslatef(0, -radius - height * 0.5, 0);
//	dome.render();
//	glPopMatrix();
//
//	//render legs
//	glPushMatrix();
//	glRotatef(90, 1, 0, 0);
//	glColor3f(0.1, 0.1, 0.1);
//
//	glPushMatrix();
//	glTranslatef(radius*0.8, 0, height * 0.5);
//	gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
//	glPopMatrix();
//
//	glPushMatrix();
//	glTranslatef(-radius * 0.8, 0, height * 0.5);
//	gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
//	glPopMatrix();
//
//	glPushMatrix();
//	glTranslatef(0, -radius * 0.8, height * 0.5);
//	gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
//	glPopMatrix();
//
//	glPushMatrix();
//	glTranslatef(0, radius * 0.8, height * 0.5);
//	gluCylinder(gluNewQuadric(), 0.25, 0.25, 3, 20, 4);
//	glPopMatrix();
//
//	glPopMatrix();
//
//	//rensder components
//	for (int i = 0; i < numComs; i++) {
//		GetComponent(i)->render();
//	}
//	glPopMatrix();
//}