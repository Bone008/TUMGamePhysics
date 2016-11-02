#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = TWO_POINT_SETUP;

	// Data Attributes
	m_fMass       = 10;
	m_fStiffness  = 40;
	m_fDamping    = 0.1f;
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
		break;

	case COMPLEX_SETUP:
		cout << "complex setup!\n";
		reset();
		// cube
		{
			// global position of cube
			int x = 0;
			int y = 0;
			int z = 0;

			// vertices
			int p000 = addMassPoint(Vec3(x + 0, y + 0, z + 0), Vec3(), true);
			int p001 = addMassPoint(Vec3(x + 0, y + 0, z + 1), Vec3(), false);
			int p010 = addMassPoint(Vec3(x + 0, y + 1, z + 0), Vec3(), false);
			int p011 = addMassPoint(Vec3(x + 0, y + 1, z + 1), Vec3(), false);
			int p100 = addMassPoint(Vec3(x + 1, y + 0, z + 0), Vec3(), false);
			int p101 = addMassPoint(Vec3(x + 1, y + 0, z + 1), Vec3(), false);
			int p110 = addMassPoint(Vec3(x + 1, y + 1, z + 0), Vec3(), false);
			int p111 = addMassPoint(Vec3(x + 1, y + 1, z + 1), Vec3(), false);
			
			// edges
			addSpring(p000, p001, 2);
			addSpring(p000, p010, 2);
			addSpring(p000, p100, 2);
			addSpring(p001, p011, 2);
			addSpring(p001, p101, 2);
			addSpring(p010, p011, 2);
			addSpring(p010, p110, 2);
			addSpring(p011, p111, 2);
			addSpring(p100, p101, 2);
			addSpring(p100, p110, 2);
			addSpring(p101, p111, 2);
			addSpring(p110, p111, 2);

			// diagonals
			float diagLength = sqrt(3) * 2;
			addSpring(p000, p111, diagLength);
			addSpring(p001, p110, diagLength);
			addSpring(p010, p101, diagLength);
			addSpring(p011, p100, diagLength);
		}

		// tetrahedron
		{
			// global position of tetrahedron
			int x = 2;
			int y = 2;
			int z = 2;

			// 4 corners
			int a = addMassPoint(Vec3(x + 1, y + 1, z - 1), Vec3(), false);
			int b = addMassPoint(Vec3(x - 1, y - 1, z - 1), Vec3(), false);
			int c = addMassPoint(Vec3(x - 1, y + 1, z + 1), Vec3(), false);
			int d = addMassPoint(Vec3(x + 1, y - 1, z + 1), Vec3(), false);

			// edges
			addSpring(a, b, 2);
			addSpring(a, c, 2);
			addSpring(a, d, 2);
			addSpring(b, c, 2);
			addSpring(b, d, 2);
			addSpring(c, d, 2);
		}
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

	// subtract damping factor
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		m_massPoints[i].force -= m_fDamping * getVelocityOfMassPoint(i);
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

		if (p->isFixed)
			continue;

		// calculate acceleration a = f/m
		Vec3 acceleration = p->force / p->mass;

		// calculate position from velocity and delta t
		p->position += p->velocity * timeStep;

		// calculate velocity from acceleration and delta t
		p->velocity += acceleration * timeStep;
	}
}

void MassSpringSystemSimulator::integrateMidpoint(float timeStep)
{
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		point* p = &m_massPoints[i];

		if (p->isFixed)
			continue;

		// calculate acceleration a = f/m
		Vec3 acceleration = p->force / p->mass;

		// calculate midpoint (delta t / 2)
		// calculate velocity (position) from acceleration (velocity)
		p->position += p->velocity * (timeStep / 2);
		p->velocity += acceleration * (timeStep / 2);

		// recompute elastic forces based on midpoint
		computeElasticForces();

		// calculate acceleration a = f/m
		acceleration = p->force / p->mass;

		// calculate final point from force of midpoint
		// calculate velocity (position) from acceleration (velocity)
		p->position += p->velocity * timeStep;
		p->velocity += acceleration * timeStep;
	}
}
