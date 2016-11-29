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
	RigidBody() : RigidBody(Vec3(0, 0, 0), Quat(0, 0, 0, 1), Vec3(1, 1, 1), 1.0f) {}
	RigidBody(Vec3 position, Quat orientation, Vec3 size, float mass);

	Vec3 m_position;        //x
	Quat m_orientation;     //r
	Vec3 m_linearVelocity;  //v
	Vec3 m_angularMomentum; //L
	
	Vec3 m_size;
	float m_bounceFactor;   //c
	float m_mass;           //M

	// note that this should be a Mat3, but we use a dummy 4th dimension because we don't have a Mat3
	Mat4 m_inertiaTensorInv;


	// updated each frame

	Vec3 m_angularVelocity; //w - this will be overwritten with value derived from L!

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