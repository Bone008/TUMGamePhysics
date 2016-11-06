#include "point.h"

void point::draw(DrawingUtilitiesClass * DUC)
{
	DUC->setUpLighting(Vec3(), MASS_POINT_COLOR, 100, MASS_POINT_COLOR);
	DUC->drawSphere(position, Vec3(MASS_POINT_SIZE, MASS_POINT_SIZE, MASS_POINT_SIZE));
}

void point::clearForce()
{
	force = Vec3();
}

void point::resetPosition()
{
	position = initPosition;
}


void point::addGravity(float timeStep)
{
	velocity.y += -GRAVITY*timeStep;
	//as with the example from the lecture - this should be here.
	//position += ((float)(-1 / 2))*GRAVITY*timeStep*timeStep;
}

void point::removeGravity()
{
	velocity.y += 0;
}
