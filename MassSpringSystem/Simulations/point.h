#ifndef UTIL_POINT_H
#define UTIL_POINT_H

#include "Simulator.h"

#define MASS_POINT_SIZE 0.02
#define MASS_POINT_COLOR Vec3(0, 1, 1)

class point {
public:
	Vec3 position;
	Vec3 velocity;
	Vec3 force;
	float mass;

	bool isFixed;

	// suggested in lecture but not used yet
	/*	
	float damping;
	*/

	void draw(DrawingUtilitiesClass * DUC);
	void clearForce();
};

#endif