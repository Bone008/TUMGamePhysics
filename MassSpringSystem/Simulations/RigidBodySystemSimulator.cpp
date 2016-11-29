#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	//UI attributes
	m_externalForce = Vec3();
	m_mouse			= Point2D();
	m_trackmouse	= Point2D();
	m_oldtrackmouse = Point2D();
}

// Functions
const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "One-step test, Single rigid body, Two-rigid-body, Complex Simulation";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	switch (m_iTestCase)
	{
		case ONE_STEP_SIMULATION: // no ui needed

		//TODO Add interfaces
		case SINGLE_BODY_SIMULATION:
		case DOUBLE_BODY_SIMULATION:
		case COMPLEX_BODY_SIMULATION:
			break;
	}
}

void RigidBodySystemSimulator::reset()
{
	r_rigidBodies.clear();
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	for (const RigidBody& rb : r_rigidBodies)
		rb.draw(DUC);
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
		//TODO finish it (Konstantin)
		case ONE_STEP_SIMULATION: 
		case SINGLE_BODY_SIMULATION:
		case DOUBLE_BODY_SIMULATION:
		case COMPLEX_BODY_SIMULATION:
			reset();
			addRigidBody(Vec3(0,0,0),Vec3(1,1,1), 0);
	}
	
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
}

void RigidBodySystemSimulator::onClick(int x, int y)
{
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
}

// ExtraFunctions
int RigidBodySystemSimulator::getNumberOfRigidBodies()
{
	return 0;
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i)
{
	return Vec3();
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	return Vec3();
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i)
{
	return Vec3();
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	RigidBody rb = RigidBody(position, size, mass);
	//push it to the vector of rigid bodies
	r_rigidBodies.push_back(rb);
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
}
