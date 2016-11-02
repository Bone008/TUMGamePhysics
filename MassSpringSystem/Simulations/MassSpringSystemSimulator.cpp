#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
}

// Functions
const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Mass spring system using Euler and Midpoint";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
}

void MassSpringSystemSimulator::reset()
{
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	// draw all mass points
	for each(point tmpPoint in m_massPoints)
	{
		tmpPoint.draw(DUC);
	}

	// draw all springs
	for each(spring tmpSpring in m_springs)
	{
		tmpSpring.draw(DUC);
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 1:
		cout << "1: Simple one-step test!\n";
		break;

	case 2:
		cout << "2: Simple Euler simulation!\n";
		
		// simple scenario using euler
		setMass(10);
		setStiffness(40);

		// TODO setting the integrator here doesnt make sense:
		// in the test cases, the integrator is set BEFORE calling this function
		// so it will get overwritten this way
		setIntegrator(EULER);

		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);		

		break;

	case 3:
		cout << "3: Simple Midpoint simulation!\n";

		// simple scenario using euler
		setMass(10);
		setStiffness(40);
		setIntegrator(MIDPOINT);

		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);

		break;

	case 4:
		cout << "4: Complex simulation, stability comparison!\n";
		break;

	case 5:
		cout << "5: Leap-Frog method!\n";
		break;

	default:
		cout << "Empty test!\n";
		notifyCaseChanged(3); // just for testing purpose TODO remove
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	
	// integrate position from forces
	switch (m_iTestCase)
	{
	case 2:
		clearForces();
		computeElasticForces();
		integrateEuler(timeStep);
		break;

	case 3:
		clearForces();
		computeElasticForces();
		integrateMidpoint(timeStep);
		break;

	default:
		break;
	}
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
}

// Specific Functions
void MassSpringSystemSimulator::setIntegrator(int integrator)
{
	m_iIntegrator = integrator;
}

void MassSpringSystemSimulator::setMass(float mass)
{
	m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
	m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed)
{
	// create new mass point from parameters
	point newMassPoint;
	newMassPoint.position = position;
	newMassPoint.velocity = velocity;
	newMassPoint.force = Vec3();
	newMassPoint.mass = m_fMass;
	newMassPoint.isFixed = isFixed;

	// push new mass point to vector of mass points
	m_massPoints.push_back(newMassPoint);

	// return index of new mass point in vector
	return m_massPoints.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	// create new spring from parameters
	spring newSpring;
	newSpring.point1 = masspoint1;
	newSpring.point2 = masspoint2;
	newSpring.initialLength = initialLength;
	newSpring.stiffness = m_fStiffness;
	newSpring.m_massPoints = &m_massPoints;

	// push new spring to vector of springs
	m_springs.push_back(newSpring);
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return m_massPoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return m_springs.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return m_massPoints.at(index).position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return m_massPoints.at(index).velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
}


// Force and integrating functions
void MassSpringSystemSimulator::clearForces()
{
	// clear force of every point
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		m_massPoints[i].clearForce();
	}
}

void MassSpringSystemSimulator::computeElasticForces()
{
	// calculate forces of each point
	for (spring currentSpring : m_springs)
	{
		currentSpring.computeElasticForces();
	}
}

void MassSpringSystemSimulator::integrateEuler(float timeStep)
{
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		point* p = & m_massPoints[i];

		// calculate acceleration a = f/m
		Vec3 acceleration = p->force / p->mass;

		// TODO correct order?
		// calculate velocity from acceleration and delta t
		p->velocity += acceleration * timeStep;

		// calculate position from velocity and delta t
		p->position += p->velocity * timeStep;
	}
}

void MassSpringSystemSimulator::integrateMidpoint(float timeStep)
{
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		point* p = &m_massPoints[i];

		// calculate acceleration a = f/m
		Vec3 acceleration = p->force / p->mass;

		// calculate midpoint (delta t / 2)
		// calculate velocity (position) from acceleration (velocity)
		p->velocity += acceleration * (timeStep / 2);
		p->position += p->velocity * (timeStep / 2);

		// recompute elastic forces based on midpoint
		computeElasticForces();

		// calculate acceleration a = f/m
		acceleration = p->force / p->mass;

		// calculate final point from force of midpoint
		// calculate velocity (position) from acceleration (velocity)
		p->velocity += acceleration * timeStep;
		p->position += p->velocity * timeStep;
	}
}
