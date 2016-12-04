#include "RigidBody.h"

RigidBody::RigidBody(Vec3 position, Quat orientation, Vec3 size, float mass) : 
	m_position(position), m_orientation(orientation), m_size(size), m_mass(mass)
{
	// calculate initial inertia tensor according to https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
	double w = m_size.x;
	double h = m_size.y;
	double d = m_size.z;

	Mat4 inertiaTensor;
	inertiaTensor.value[0][0] = m_mass / 12.0 * (h*h + d*d);
	inertiaTensor.value[1][1] = m_mass / 12.0 * (w*w + d*d);
	inertiaTensor.value[2][2] = m_mass / 12.0 * (h*h + h*h);
	inertiaTensor.value[3][3] = 1;

	m_inertiaTensorInv = inertiaTensor.inverse();

	updateObjToWorldMatrix();
}

// private, has to be called whenever the position changes
void RigidBody::updateObjToWorldMatrix()
{
	Mat4 translateMatrix, rotationMatrix, scaleMatrix;
	translateMatrix.initTranslation(m_position.x, m_position.y, m_position.z);
	rotationMatrix = m_orientation.getRotMat();
	scaleMatrix.initScaling(m_size.x, m_size.y, m_size.z);
	
	m_objToWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
}

void RigidBody::resetExternalForces()
{
	m_externalForces = Vec3();
	m_externalTorque = Vec3();
}

void RigidBody::applyExternalForce(Vec3 force, Vec3 forcePosition)
{
	m_externalForces += force;
	m_externalTorque += cross((forcePosition - m_position), force);
}


void RigidBody::integrateTimestep(float timeStep)
{
	// integrate values according to simulation algorithm given on slides
	// x
	m_position += timeStep * m_linearVelocity;
	// v
	m_linearVelocity += timeStep * m_externalForces / m_mass;
	// r
	m_orientation += (Quat(m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 0) * m_orientation) * (timeStep * 0.5);
	m_orientation /= m_orientation.norm(); // normalize; this acould be done more seldomly if performance becomes an issue

	// L
	m_angularMomentum += timeStep * m_externalTorque;
	
	Mat4 rotationMatrix(m_orientation.getRotMat());
	Mat4 rotationMatrixInv(rotationMatrix);
	rotationMatrixInv.transpose(); // transpose() is in-place for some reason, while inverse() is not ...
	// I^-1
	Mat4 rotatedInertialTensorInv = rotationMatrix * m_inertiaTensorInv * rotationMatrixInv;

	// w
	m_angularVelocity = rotatedInertialTensorInv * m_angularMomentum;

	updateObjToWorldMatrix();
}

void RigidBody::draw(DrawingUtilitiesClass * DUC) const
{
	DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
	DUC->drawRigidBody(m_objToWorldMatrix);
}

//this the x vector, as described in the lecture, pointing from the object center to the point of act
Vec3 RigidBody::getCenterToPoint(Vec3 pointOfAct)
{
	return pointOfAct - m_position;
}
