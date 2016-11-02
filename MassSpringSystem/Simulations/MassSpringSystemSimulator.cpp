#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = TWO_POINT_SETUP;

	// Data Attributes
	m_fMass       = 10;
	m_fStiffness  = 40;
	m_fDamping    = 0;
	m_iIntegrator = EULER;

	// UI Attributes
	m_externalForce = Vec3();
	m_mouse         = Point2D();
	m_trackmouse    = Point2D();
	m_oldtrackmouse = Point2D();
}

// Functions
const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "One-step test,2 point setup,complex setup";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case ONE_STEP_TEST: // no ui needed
		break;

	case TWO_POINT_SETUP:
	case COMPLEX_SETUP:
		// add integrator-selection
		TwAddSeparator(DUC->g_pTweakBar, "Separator", "");
		TwEnumVal integrators[] =
		{
			{ EULER, "Euler" },
			{ MIDPOINT, "Midpoint" },
			{ LEAPFROG, "Leapfrog" }
		};
		TwType integratorType = TwDefineEnum("Integrator", integrators, 3);
		TwAddVarRW(DUC->g_pTweakBar, "Integrator", integratorType, &m_iIntegrator, "");


	}
}

void MassSpringSystemSimulator::reset()
{
	// delete all mass points and springs
	m_springs.clear();
	m_massPoints.clear();
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
	case ONE_STEP_TEST:
		cout << "Simple one-step test!\n";
		// TODO print calculations to console
		break;

	case TWO_POINT_SETUP:
		cout << "2-point setup!\n";
		reset();
		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);
		setMass(0.01f);
		setStiffness(25.0f);
		setDampingFactor(0.01f);
		setIntegrator(MIDPOINT);
		for (int i = 0; i <10; i++)
			simulateTimestep(0.005);

		cout << "p1 " << getPositionOfMassPoint(0) << "\n";
		cout << "p2 " << getPositionOfMassPoint(1) << "\n";
		break;

	case COMPLEX_SETUP:
		cout << "complex setup!\n";
		// TODO think of complex example (10+ mass points, 10+ springs)
		break;

	default:
		cout << "Empty test!\n";
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
	case ONE_STEP_TEST: // no simulation, just console output
		break;

	case TWO_POINT_SETUP:
	case COMPLEX_SETUP:
		clearForces();
		computeElasticForces();
		integrate(timeStep);
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

void MassSpringSystemSimulator::integrate(float timeStep)
{
	switch (m_iIntegrator)
	{
	case EULER:
		integrateEuler(timeStep);
		break;

	case MIDPOINT:
		integrateMidpoint(timeStep);
		break;

	case LEAPFROG:
		// TODO
		break;
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
