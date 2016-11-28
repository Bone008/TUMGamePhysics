#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "point.h"
#include "spring.h"
#include "util\quaternion.h"

class RigidBodySystem {

	RigidBodySystem() : r_center(Vec3()), r_size(Vec3()), r_mass(0), r_orientation(Real()) {}

	RigidBodySystem(Vec3 center, Vec3 size, float mass, Quat orientation);

	// Masspoints and Springs
	std::vector<point> m_massPoints;
	std::vector<spring> m_springs;

	//Rigid body attributes
	Vec3 r_position;		//x
	Vec3 r_orientation;		//r
	Vec3 r_linearVelocity;	//v
	Vec3 r_angularVelocity; //w

	Vec3 r_center;
	Vec3 r_size;
	Quat r_orientation;

	float r_bounceFactor;	//c
	float r_mass;			//m

};
#endif
