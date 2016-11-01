#ifndef UTIL_POINT_H
#define UTIL_POINT_H

#include "Simulator.h"

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

	void clearForce();
};

#endif