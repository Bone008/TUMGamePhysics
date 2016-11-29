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

	// TODO store initial inertial tensor I as well

	// updated each frame
	Mat4 m_objToWorldMatrix;
	Vec3 m_externalForces; // F
	Vec3 m_externalTorque; // q


	// reset the external force accumulators (F and q)
	void resetExternalForces();
	// add an external force to the accumulators (F and q)
	// forcePosition is given in world space
	void applyExternalForce(Vec3 force, Vec3 forcePosition);

	void integrateTimestep(float timeStep);
	void draw(DrawingUtilitiesClass * DUC) const;

};