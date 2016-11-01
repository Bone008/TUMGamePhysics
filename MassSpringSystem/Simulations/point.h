#ifndef UTIL_POINT_H
#define UTIL_POINT_H

#include "Simulator.h"

class point {
public:
	Vec3 position;
	Vec3 velocity;
	bool isFixed;

	// defined in lecture but not used yet
	/*	
	Vec3 force;
	float mass;
	float damping;
	*/
};

#endif