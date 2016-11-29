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
	RigidBody() : RigidBody(Vec3(0, 0, 0), Vec3(1, 1, 1), 1.0f) {}

	RigidBody(Vec3 position, Vec3 size, float mass) : 
		m_position(position), m_size(size), m_mass(mass) {}

	Vec3 m_position;        //x
	Quat m_orientation;     //r
	Vec3 m_linearVelocity;  //v
	Vec3 m_angularVelocity; //w
	
	Vec3 m_size;
	float m_bounceFactor;   //c
	float m_mass;           //M

	// updated each frame
	Mat4 m_objToWorldMatrix;

	void integrateTimestep(float timeStep);
	void draw(DrawingUtilitiesClass * DUC) const;

};