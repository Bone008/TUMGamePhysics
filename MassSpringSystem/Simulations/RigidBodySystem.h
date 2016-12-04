#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodySystem {

	//according to lecture 5
	inline void onCollisionLinearVelocitiesUpdate(RigidBody a, RigidBody b) {
		a.m_angularVelocity += (a.m_impulseForce*a.m_surfaceNormal) / a.m_mass;
		b.m_angularVelocity += (b.m_impulseForce*b.m_surfaceNormal) / b.m_mass;
	}

	//according to lecture 5
	inline void onCollisionAngularMomentumUpdate(RigidBody a, RigidBody b) {
		a.m_angularMomentum += cross(a.m_centerToCollisionPoint, (a.m_impulseForce*a.m_surfaceNormal)) / a.m_inertiaTensorInv.inverse;
		b.m_angularMomentum += cross(b.m_centerToCollisionPoint, (b.m_impulseForce*b.m_surfaceNormal)) / b.m_inertiaTensorInv.inverse;
	}

	//according to lecture 5
	inline void onCollisionImpulsesUpdate(RigidBody a, RigidBody b) {
		a.m_impulseForce = dot((-(1 + a.m_bounciness)*a.m_angularVelocity), a.m_surfaceNormal) /
								((1 / a.m_mass) + (1 / b.m_mass));

		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
			((1 / a.m_mass) + (1 / b.m_mass));					
	}


};
#endif
