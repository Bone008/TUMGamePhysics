#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodySystem {
public:
	inline void onCollision(RigidBody &a, RigidBody &b, Vec3 collisionPoint, bool bIsFixed)
	{
		// calculate impulse
		double c = a.m_bounciness; // ? a or b
		Vec3 n = a.m_surfaceNormal;
		Vec3 vRel = a.m_linearVelocity - (bIsFixed ? Vec3() : b.m_linearVelocity);
		Vec3 bInertiaPosN = bIsFixed ? Vec3() : cross(b.m_inertiaTensorInv * cross(b.m_position, n), b.m_position);
		double bMassInv = bIsFixed ? 0 : (1 / b.m_mass);

		// TODO somehow make readable? nominee for ugliest formula alive
		double J = dot(-(1 + c) * vRel, n) / (1 / a.m_mass + bMassInv + dot(
			cross(a.m_inertiaTensorInv * cross(a.m_position, n), a.m_position)
			+ bInertiaPosN
			, n));

		assert(J >= 0); // TODO remove

		// "precompute"
		Vec3 impulseNormal = J * n;

		// linear velocity and angular momentum for a
		a.m_linearVelocity += impulseNormal / a.m_mass;
		a.m_angularMomentum += cross(a.m_position, impulseNormal); // TODO is a.m_position correct? i got better results when using the collisionPoint here

		// same for b
		if (!bIsFixed) {
			b.m_angularMomentum -= cross(b.m_position, impulseNormal); // TODO is b.m_position correct? i got better results when using the collisionPoint here
			b.m_linearVelocity -= impulseNormal / b.m_mass;
		}
	}

};
#endif
