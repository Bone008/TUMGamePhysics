#pragma once
#include "Simulator.h"

#include "util\matrixbase.h"
#include "point.h"
#include "spring.h"
#include "util\quaternion.h"

/*	Simple Rigid body class for creation and handling 
	of 3D Rigid bodies in form of cubes.
*/
class RigidBody {
public:
	RigidBody() : r_center(Vec3()), r_size(Vec3()), r_mass(0), r_orientation(Real()) {}

	//TODO Konstantin
	RigidBody(Vec3 position, Vec3 size, float mass):
		r_position(position), r_size(size), r_mass(mass) {}

	// Masspoints
	std::vector<point> m_massPoints;

	//Rigid body attributes. VECTORS might be deleted but for now I'll leave them here
	Vec3 r_position;		//x
	Vec3 r_orientation;		//r
	Vec3 r_linearVelocity;	//v
	Vec3 r_angularVelocity; //w

	Mat4 r_wolrdPositionMatrix;
	Mat4 r_scaleMatrix;
	Mat4 r_tranlsateMatrix;
	Mat4 r_rotationMatrix;

	Vec3 r_center;
	Vec3 r_size;
	Quat r_quatOrientation;

	float r_bounceFactor;	//c
	float r_mass;			//m

	//Forces
	Vec3 f_gravity;
	Vec3 f_mouseForce;
	Quat f_rotationForce;

	void draw(DrawingUtilitiesClass * DUC) const;
	void clearRotaionForce();
	void clearMouseForce();
	void clearGravity();

};