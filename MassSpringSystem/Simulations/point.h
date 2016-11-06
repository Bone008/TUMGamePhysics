#ifndef UTIL_POINT_H
#define UTIL_POINT_H

#include "Simulator.h"

#define MASS_POINT_SIZE 0.02
#define MASS_POINT_COLOR Vec3(0, 1, 1)
#define GRAVITY 2

class point {
public:
	Vec3 position;
	Vec3 velocity;
	Vec3 force;
	//test
	Vec3 initPosition;
	Vec3 initVelocity;

	float mass;

	bool isFixed;

	// suggested in lecture but not used yet
	/*	
	float damping;
	*/

	void draw(DrawingUtilitiesClass * DUC);
	void clearForce();
	void addGravity(float timeStep);
};

#endif