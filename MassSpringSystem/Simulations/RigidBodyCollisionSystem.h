#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodyCollisionSystem {
public:
	inline void testCollision(RigidBody& a, RigidBody& b, bool bIsFixed)
	{
		CollisionInfo collInfo = checkCollisionSAT(a.m_objToWorldMatrix, b.m_objToWorldMatrix);
		if (!collInfo.isValid)
			return;

		cout << "Collision!";

		// collision point in object space of A and B
		Vec3 collisionPointA = collInfo.collisionPointWorld - a.m_position;
		Vec3 collisionPointB = collInfo.collisionPointWorld - b.m_position;
		// relative velocity
		Vec3 va = a.m_linearVelocity + cross(a.m_angularVelocity, collisionPointA);
		Vec3 vb = b.m_linearVelocity + cross(b.m_angularVelocity, collisionPointB);
		Vec3 vRel = va - vb;

		// vRel * n
		double separationIndicator = dot(vRel, collInfo.normalWorld);

		// ignore separating bodies
		if (separationIndicator > 0) {
			cout << " ignored" << endl;
			return;
		}

		cout << " accepted" << endl;

		
		// calculate impulse
		double c = a.m_bounciness; // ? a or b
		Vec3 n = collInfo.normalWorld;

		// calculate J
		Vec3 rotationThingA = cross(a.m_inertiaTensorInv * cross(collisionPointA, n), collisionPointA);
		Vec3 rotationThingB = bIsFixed ? Vec3() : cross(b.m_inertiaTensorInv * cross(collisionPointB, n), collisionPointB);
		double aMassInv = (1 / a.m_mass);
		double bMassInv = bIsFixed ? 0 : (1 / b.m_mass);

		double J =
			dot(-(1 + c) * vRel, n)
			           /
			(aMassInv + bMassInv + dot(rotationThingA + rotationThingB, n));

		assert(J >= 0);

		// "precompute"
		Vec3 impulseNormal = J * n;

		// linear velocity and angular momentum for a
		a.m_linearVelocity += impulseNormal / a.m_mass;
		a.m_angularMomentum += cross(collisionPointA, impulseNormal);

		// same for b
		if (!bIsFixed) {
			b.m_angularMomentum -= cross(collisionPointB, impulseNormal);
			b.m_linearVelocity -= impulseNormal / b.m_mass;
		}
	}

};
#endif
