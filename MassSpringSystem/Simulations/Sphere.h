#ifndef SPHERE_h
#define SPHERE_h

#include "util\vectorbase.h"
#include "DrawingUtilitiesClass.h"

struct Sphere
{
	Vec3 pos;
	Vec3 vel;
	Vec3 computedForce;

	float radius;
};

#endif
