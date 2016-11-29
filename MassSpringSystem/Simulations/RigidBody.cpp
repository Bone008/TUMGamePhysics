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
	// integrate values according to simulation algorithm given on slides
	m_position += timeStep * m_linearVelocity;
	m_linearVelocity += timeStep * m_externalForces / m_mass;
	m_orientation += (Quat(m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 0) * m_orientation) * (timeStep * 0.5);

	// TODO integrate L, update I, integrate w
	m_angularVelocity = Vec3(0, 1, 1); // test value for w


	// update obj to world matrix
	Mat4 translateMatrix, rotationMatrix, scaleMatrix;
	translateMatrix.initTranslation(m_position.x, m_position.y, m_position.z);
	rotationMatrix = m_orientation.getRotMat();
	scaleMatrix.initScaling(m_size.x, m_size.y, m_size.z);

	m_objToWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
}

void RigidBody::draw(DrawingUtilitiesClass * DUC) const
{
	DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
	DUC->drawRigidBody(m_objToWorldMatrix);
}
