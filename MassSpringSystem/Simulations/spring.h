#ifndef UTIL_SPRING_H
#define UTIL_SPRING_H

#include "point.h"

#define SPRING_COLOR Vec3(1, 1, 0)

class spring {
public:
	int point1;
	int point2;
	float initialLength;
	float stiffness;

	std::vector<point>* m_massPoints;

	// suggested in lecture but not used yet
	/*
	float currentLength;
	*/

	void draw(DrawingUtilitiesClass * DUC);
	void computeElasticForces();
};

#endif