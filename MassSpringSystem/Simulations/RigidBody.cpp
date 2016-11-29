#include "RigidBody.h"

void RigidBody::integrateTimestep(float timeStep)
{
	// TODO change position/orientation and stuff

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
