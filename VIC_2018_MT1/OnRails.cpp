#include "OnRails.h"

OnRails::OnRails(float an, char ax, float translationX, float translationY, float translationZ, float frameRate, float RunningTime)
{
	ang = an;
	axis = ax;
	transX = translationX;
	transY = translationY;
	transZ = translationZ;
	MaxTime = RunningTime;
	fRate = frameRate;
}

OnRails::OnRails(float frameRate)
{
	fRate = frameRate;
	ang = 0;
	axis = 'n';
	transX = 0;
	transY = 0;
	transZ = 0;
	MaxTime = 100;
}

void OnRails::ApplyAffine()
{
	float factor = RunTime / MaxTime;

	if (axis == 'X' || axis == 'x') {
		glRotatef(ang * factor, 1, 0, 0);
	}
	else if (axis == 'Y' || axis == 'y') {
		glRotatef(ang * factor, 0, 1, 0);
	}
	else if (axis == 'Z' || axis == 'z') {
		glRotatef(ang * factor, 0, 0, 1);
	}

	glTranslatef(transX * factor, transY * factor, transZ * factor);
}

void OnRails::render()
{
	updateTime();
	glPushMatrix();
	ApplyAffine();

	renderSelf();
	for (int i = 0; i < numChildren; i++) {
		children[i]->render();
	}


	glPopMatrix();

}

void OnRails::addChild(OnRails *child)
{
	children[numChildren] = child;
	numChildren++;
}

void OnRails::StartChildren()
{
	for (int i = 0; i < numChildren; i++) {
		children[i]->Start();
	}
}

void OnRails::Start()
{
	IsStarted = true;
	if (StartChildrenOnStart)
		StartChildren();
}

void OnRails::updateTime()
{
	if (!IsStarted)
		return;

	if (!IsFinished()) {
		RunTime += 1.0/fRate;
		if (IsFinished() && StartChildrenOnFinish)
		{
			StartChildren();
		}
	}
	else if (Restart) {
		RunTime = 0;
	}
	else
	{
		return;
	}
}

bool OnRails::IsFinished()
{
	return (MaxTime < RunTime*1.0001);
}
