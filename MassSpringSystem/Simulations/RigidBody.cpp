#include "RigidBody.h"

void RigidBody::resetExternalForces()
{
	m_externalForces = Vec3();
	m_externalTorque = Vec3();
}

void RigidBody::applyExternalForce(Vec3 force, Vec3 forcePosition)
{
	m_externalForces += force;
	//m_externalTorque += ???; TODO figure out which "x_i" to use for torque calcuation
}

void RigidBody::integrateTimestep(float timeStep)
{
	// TODO integrate position/orientation and stuff

	// update obj to world matrix
	Mat4 translateMatrix, rotationMatrix, scaleMatrix;
	translateMatrix.initTranslation(m_position.x, m_position.y, m_position.z);
	rotationMatrix.initRotationXYZ(m_orientation.x, m_orientation.y, m_orientation.z);
	scaleMatrix.initScaling(m_size.x, m_size.y, m_size.z);

	m_objToWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
}

void RigidBody::draw(DrawingUtilitiesClass * DUC) const
{
	DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
	DUC->drawRigidBody(m_objToWorldMatrix);
}
