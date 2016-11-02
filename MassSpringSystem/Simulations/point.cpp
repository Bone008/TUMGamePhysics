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
