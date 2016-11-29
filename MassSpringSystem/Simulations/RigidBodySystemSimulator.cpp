#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	//UI attributes
	m_externalForce = Vec3(); // not sure if still useful
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
			applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1, 1, 0));
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
	// TODO per frame calls to RigidBody.resetExternalForces() and RigidBody.applyExternalForce(...) will go here
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	for (RigidBody& rb : m_rigidBodies)
		rb.integrateTimestep(timeStep);
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
	m_rigidBodies[i].applyExternalForce(force, loc);
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



void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	RigidBody rb(position, size, mass);
	//push it to the vector of rigid bodies
	m_rigidBodies.push_back(rb);
}




// other functions (mostly for the unit tests)
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

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_rigidBodies[i].m_orientation = orientation;
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_rigidBodies[i].m_linearVelocity = velocity;
}
