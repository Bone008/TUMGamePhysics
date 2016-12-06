#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodySystem {
public:
	//according to lecture 5. Should be called second/third
	inline void onCollisionLinearVelocitiesUpdate(RigidBody a, RigidBody b) {
		a.m_angularVelocity += (a.m_impulseForce*a.m_surfaceNormal) / a.m_mass;
		b.m_angularVelocity -= (b.m_impulseForce*b.m_surfaceNormal) / b.m_mass;
	}

	
	//according to lecture 5. Should be called second/third
	inline void onCollisionAngularMomentumUpdate(RigidBody a, RigidBody b) {
		a.m_angularMomentum += cross(a.m_position, (a.m_impulseForce*a.m_surfaceNormal));
		b.m_angularMomentum -= cross(b.m_position, (b.m_impulseForce*b.m_surfaceNormal));
	}
	
	//according according to what the prof showed us. Should be called first
	inline void onCollisionImpulsesUpdate(RigidBody a, RigidBody b) {
		a.m_impulseForce = dot((-(1 + a.m_bounciness)*a.m_angularVelocity), a.m_surfaceNormal) /
								((1 / a.m_mass) + (1 / b.m_mass));

		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
								((1 / a.m_mass) + (1 / b.m_mass));					
	}

	//according to what the prof showed us. Should be called first
	inline void onCollisionWithWallImpulsesUpdate(RigidBody b) {
		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
			(1 / b.m_mass);
	}
	//according to lecture 5. Should be called first
	inline void onCollisionWithWallImpulsesUpdateVersion2(RigidBody b) {
		b.m_impulseForce = dot((-(1 + b.m_bounciness)*b.m_angularVelocity), b.m_surfaceNormal) /
			(1 / b.m_mass + dot(cross(b.m_inertiaTensorInv*(cross(b.m_position,b.m_surfaceNormal)),b.m_position)
								, b.m_surfaceNormal));
	}
	//according to lecture 5. Should be called second/third
	inline void onCollisionWithWallLinearVelocitiesUpdate(RigidBody b) {
		b.m_linearVelocity -= (b.m_impulseForce*b.m_surfaceNormal) / b.m_mass;
	}


	//according to lecture 5. Should be called second/third
	inline void onCollisionWithWallAngularMomentumUpdate(RigidBody b) {
		b.m_angularMomentum -= cross(b.m_centerToCollisionPoint, (b.m_impulseForce*b.m_surfaceNormal));
	}

	inline void onCollisionWithWall(RigidBody b) {
		onCollisionWithWallImpulsesUpdate(b);
		onCollisionWithWallLinearVelocitiesUpdate(b);
		onCollisionWithWallAngularMomentumUpdate(b);
		
	}

	inline void onCollisionTwoRigids(RigidBody a, RigidBody b) {
		onCollisionImpulsesUpdate(a, b);
		onCollisionLinearVelocitiesUpdate(a, b);
		onCollisionAngularMomentumUpdate(a, b);
	}
};
#endif
