#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodySystem {
public:
	//according to lecture 5
	inline void onCollisionLinearVelocitiesUpdate(RigidBody a, RigidBody b) {
		a.m_angularVelocity += (a.m_impulseForce*a.m_surfaceNormal) / a.m_mass;
		b.m_angularVelocity -= (b.m_impulseForce*b.m_surfaceNormal) / b.m_mass;
	}

	
	//according to lecture 5
	inline void onCollisionAngularMomentumUpdate(RigidBody a, RigidBody b) {
		a.m_angularMomentum += cross(a.m_centerToCollisionPoint, (a.m_impulseForce*a.m_surfaceNormal));
		b.m_angularMomentum -= cross(b.m_centerToCollisionPoint, (b.m_impulseForce*b.m_surfaceNormal));
	}
	
	//according to lecture 5
	inline void onCollisionImpulsesUpdate(RigidBody a, RigidBody b) {
		a.m_impulseForce = dot((-(1 + a.m_bounciness)*a.m_angularVelocity), a.m_surfaceNormal) /
								((1 / a.m_mass) + (1 / b.m_mass));

		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
								((1 / a.m_mass) + (1 / b.m_mass));					
	}

	//according to lecture 5
	inline void onCollisionWithWallLinearVelocitiesUpdate(RigidBody b) {
		b.m_angularVelocity = -(b.m_impulseForce*b.m_surfaceNormal) / b.m_mass;
	}


	//according to lecture 5
	inline void onCollisionWithWallAngularMomentumUpdate(RigidBody b) {
		b.m_angularMomentum = -cross(b.m_centerToCollisionPoint, (b.m_impulseForce*b.m_surfaceNormal));
	}

	//according to lecture 5
	inline void onCollisionWithWallImpulsesUpdate(RigidBody b) {
		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
			(1 / b.m_mass);
	}

	inline void onCollisionWithWall(RigidBody b) {
		onCollisionWithWallImpulsesUpdate(b);
		onCollisionWithWallLinearVelocitiesUpdate(b);
		onCollisionWithWallAngularMomentumUpdate(b);
		
	}

	inline void onCollisionTwoRigids(RigidBody a, RigidBody b) {
		onCollisionLinearVelocitiesUpdate(a, b);
		onCollisionAngularMomentumUpdate(a, b);
		onCollisionImpulsesUpdate(a, b);
	}
};
#endif
