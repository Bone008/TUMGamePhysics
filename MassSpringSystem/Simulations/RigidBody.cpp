#include "RigidBody.h"

void RigidBody::draw(DrawingUtilitiesClass * DUC) const
{
	DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
	//TODO FIX ME. Here doesn't work the multiplication.
	//r_wolrdPositionMatrix = (r_scaleMatrix * r_rotationMatrix * r_tranlsateMatrix);
	DUC->drawRigidBody(Mat4());
}

void RigidBody::clearRotaionForce()
{
	f_rotationForce = Quat();
}

void RigidBody::clearMouseForce()
{
	f_mouseForce = Vec3();
}

void RigidBody::clearGravity()
{
	f_gravity = Vec3();
}
