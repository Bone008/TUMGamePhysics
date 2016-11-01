#include "spring.h"

void spring::computeElasticForces()
{
	// shortcut to endpoints of spring
	point* p1 = &m_massPoints.at(point1);
	point* p2 = &m_massPoints.at(point2);

	// calculate current length of spring
	float currentLength = sqrt(p1->position.squaredDistanceTo(p2->position));

	// calculate force of spring/p1
	Vec3 force = -stiffness
		* (currentLength - initialLength)
		* ((p1->position - p2->position) / currentLength);

	// store forces in points for later integration
	p1->force = force;
	p2->force = -force;
}
