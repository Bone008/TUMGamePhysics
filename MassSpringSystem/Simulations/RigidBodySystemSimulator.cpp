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
	m_rigidBodies.clear();
}


void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	reset();

	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
		case ONE_STEP_SIMULATION: 
			addRigidBody(Vec3(0, 0, 0), Vec3(1, 0.6, 0.5), 2);
			break;
		case SINGLE_BODY_SIMULATION:
		case DOUBLE_BODY_SIMULATION:
		case COMPLEX_BODY_SIMULATION:
			// TODO complex simulations
			break;
	}
	
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	for (RigidBody& rb : m_rigidBodies)
		rb.integrateTimestep(timeStep);
}

void RigidBodySystemSimulator::onClick(int x, int y)
{
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
}


void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	for (const RigidBody& rb : m_rigidBodies)
		rb.draw(DUC);
}



// ExtraFunctions
int RigidBodySystemSimulator::getNumberOfRigidBodies()
{
	return m_rigidBodies.size();
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i)
{
	return m_rigidBodies[i].m_position;
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	return m_rigidBodies[i].m_linearVelocity;
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i)
{
	return m_rigidBodies[i].m_angularVelocity;
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	RigidBody rb(position, size, mass);
	//push it to the vector of rigid bodies
	m_rigidBodies.push_back(rb);
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_rigidBodies[i].m_orientation = orientation;
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_rigidBodies[i].m_linearVelocity = velocity;
}
