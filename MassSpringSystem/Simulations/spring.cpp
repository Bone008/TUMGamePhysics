#include "spring.h"

void spring::draw(DrawingUtilitiesClass * DUC) const
{
	// get start/end points of spring
	point p1 = m_massPoints->at(point1);
	point p2 = m_massPoints->at(point2);

	// draw line between the two points
	DUC->beginLine();
	DUC->drawLine(p1.position, SPRING_COLOR, p2.position, SPRING_COLOR);
	DUC->endLine();
}

void spring::computeElasticForces(float dampingFactor)
{
	// shortcut to endpoints of spring
	point* p1 = &m_massPoints->at(point1);
	point* p2 = &m_massPoints->at(point2);

	// calculate current length of spring
	// must not be 0 to prevent division by 0 in next line
	double currentLength = max(DBL_MIN, sqrt(p1->position.squaredDistanceTo(p2->position)));

	// calculate force of spring/p1
	Vec3 force = -stiffness
		* (currentLength - initialLength)
		* ((p1->position - p2->position) / currentLength);

	// calculate damping
	Vec3 velDiff = (p1->velocity - p2->velocity);
	force -= velDiff * dampingFactor;

	// store forces in points for later integration
	p1->force += force;
	p2->force -= force;
}
