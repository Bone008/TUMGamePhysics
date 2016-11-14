#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = TWO_POINT_SETUP;

	// Data Attributes
	m_fMass       = 10;
	m_fStiffness  = 40;
	m_fDamping    = 1.0f;
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

		TwAddVarRW(DUC->g_pTweakBar, "Damp factor", TW_TYPE_FLOAT, &this->m_fDamping, "min=0.0");
		TwAddVarRW(DUC->g_pTweakBar, "Gravity force", TW_TYPE_DIR3D, &this->m_externalForce, "");
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
	for(const point& tmpPoint : m_massPoints)
	{
		tmpPoint.draw(DUC);
	}

	// draw all springs
	for(const spring& tmpSpring : m_springs)
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

		// setup scene
		reset();
		setMass(10);
		setStiffness(40);
		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);

		// calculate and print values using Euler
		integrateEuler(0.1);
		cout << "After an Euler step:" << endl;
		cout << "Point 0 vel " << getVelocityOfMassPoint(0).toString() << ", pos " << getPositionOfMassPoint(0).toString() << endl;
		cout << "Point 1 vel " << getVelocityOfMassPoint(1).toString() << ", pos " << getPositionOfMassPoint(1).toString() << endl;
		cout << endl;
		
		// setup scene
		reset();
		setMass(10);
		setStiffness(40);
		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);

		// calculate and print values using midpoint
		integrateMidpoint(0.1);
		cout << "After a midpoint step:" << endl;
		cout << "Point 0 vel " << getVelocityOfMassPoint(0).toString() << ", pos " << getPositionOfMassPoint(0).toString() << endl;
		cout << "Point 1 vel " << getVelocityOfMassPoint(1).toString() << ", pos " << getPositionOfMassPoint(1).toString() << endl;
		cout << endl;

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
			const Vec3 pos = Vec3(-0.25f, 0, 0);
			const float size = 0.3f;

			// vertices
			int p000 = addMassPoint(pos + Vec3(0, 0, 0), Vec3(), false);
			int p001 = addMassPoint(pos + Vec3(0, 0, size), Vec3(), false);
			int p010 = addMassPoint(pos + Vec3(0, size, 0), Vec3(), true);
			int p011 = addMassPoint(pos + Vec3(0, size, size), Vec3(), false);
			int p100 = addMassPoint(pos + Vec3(size, 0, 0), Vec3(), false);
			int p101 = addMassPoint(pos + Vec3(size, 0, size), Vec3(), false);
			int p110 = addMassPoint(pos + Vec3(size, size, 0), Vec3(), false);
			int p111 = addMassPoint(pos + Vec3(size, size, size), Vec3(), false);
			
			const float springLengthFac = size + 0.2f;

			// edges
			addSpring(p000, p001, springLengthFac);
			addSpring(p000, p010, springLengthFac);
			addSpring(p000, p100, springLengthFac);
			addSpring(p001, p011, springLengthFac);
			addSpring(p001, p101, springLengthFac);
			addSpring(p010, p011, springLengthFac);
			addSpring(p010, p110, springLengthFac);
			addSpring(p011, p111, springLengthFac);
			addSpring(p100, p101, springLengthFac);
			addSpring(p100, p110, springLengthFac);
			addSpring(p101, p111, springLengthFac);
			addSpring(p110, p111, springLengthFac);
		}

		// tetrahedron
		{
			// global position of tetrahedron
			const Vec3 pos = Vec3();
			const float halfSize = 0.2f;

			// 4 corners
			int a = addMassPoint(pos + Vec3(+halfSize, +halfSize, -halfSize), Vec3(), false);
			int b = addMassPoint(pos + Vec3(-halfSize, -halfSize, -halfSize), Vec3(), false);
			int c = addMassPoint(pos + Vec3(-halfSize, +halfSize, +halfSize), Vec3(), false);
			int d = addMassPoint(pos + Vec3(+halfSize, -halfSize, +halfSize), Vec3(), false);

			// edges
			addSpring(a, b, halfSize);
			addSpring(a, c, halfSize);
			addSpring(a, d, halfSize);
			addSpring(b, c, halfSize);
			addSpring(b, d, halfSize);
			addSpring(c, d, halfSize);
		}
		
		// star-like thingy
		{
			const Vec3 pos = Vec3(0.25f, 0, 0);
			const float halfSize = 0.1f;

			int m = addMassPoint((pos), Vec3(), false);
			int l = addMassPoint(pos + Vec3(-halfSize, 0, 0), Vec3(), false);
			int r = addMassPoint(pos + Vec3(+halfSize, 0, 0), Vec3(), false);
			int d = addMassPoint(pos + Vec3(0, -halfSize, 0), Vec3(), false);
			int t = addMassPoint(pos + Vec3(0, +halfSize, 0), Vec3(), false);
			int f = addMassPoint(pos + Vec3(0, 0, -halfSize), Vec3(), false);
			int b = addMassPoint(pos + Vec3(0, 0, +halfSize), Vec3(), false);

			// connect middle with other directions
			addSpring(m, l, halfSize);
			addSpring(m, r, halfSize);
			addSpring(m, d, halfSize);
			addSpring(m, t, halfSize);
			addSpring(m, f, halfSize);
			addSpring(m, b, halfSize);

			// diagonals
			const float sideLength = sqrt(2) * halfSize;
			addSpring(l, t, sideLength);
			addSpring(l, d, sideLength);
			addSpring(l, f, sideLength);
			addSpring(l, b, sideLength);
			addSpring(r, t, sideLength);
			addSpring(r, d, sideLength);
			addSpring(r, f, sideLength);
			addSpring(r, b, sideLength);
			addSpring(d, f, sideLength);
			addSpring(d, b, sideLength);
			addSpring(t, f, sideLength);
			addSpring(t, b, sideLength);
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
	case ONE_STEP_TEST:		
		break;		

	case TWO_POINT_SETUP:
	case COMPLEX_SETUP:
		clearForces();
		computeElasticForces();
		applyExternalForce(m_externalForce);
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
	for (point& p : m_massPoints)
	{
		p.force += force;
	}
}


// Force and integrating functions
void MassSpringSystemSimulator::clearForces()
{
	// clear force of every point
	for (point& p : m_massPoints)
	{
		p.clearForce();
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
	for (point& p : m_massPoints)
	{
		p.force -= m_fDamping * p.velocity;
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
	for (point& p : m_massPoints)
	{
		// ignore fixed points
		if (p.isFixed)
			continue;

		// calculate acceleration a = f/m
		Vec3 acceleration = p.force / p.mass;

		// calculate position from velocity and delta t
		p.position += p.velocity * timeStep;

		// calculate velocity from acceleration and delta t
		p.velocity += acceleration * timeStep;

		validatePointPosition(p);
	}
}

// TODO now correct?
void MassSpringSystemSimulator::integrateMidpoint(float timeStep)
{
	// create copy of masspoints
	// FIXME is this really a copy or just a copy of references?
	std::vector<point> cp_massPoints(m_massPoints);
	
	// first step: calculate midpoint values
	for (point& p : m_massPoints)
	{
		// ignore fixed points
		if (p.isFixed)
			continue;

		// calculate acceleration a = f/m
		Vec3 acceleration = p.force / p.mass;

		// calculate midpoint (delta t / 2)
		// calculate velocity (position) from acceleration (velocity)
		p.position += p.velocity * (timeStep / 2);
		p.velocity += acceleration * (timeStep / 2);

		// TODO validate point positions here, too?
	}

	// recompute elastic forces based on midpoint
	clearForces();
	computeElasticForces();
	applyExternalForce(m_externalForce);

	// step 2: compute actual values from position/velocity before step 1 (cp_massPoints) and force calculated from midpoint
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		point& p = m_massPoints[i];

		// ignore fixed points
		if (p.isFixed)
			continue;

		// calculate acceleration a = f/m
		Vec3 acceleration = p.force / p.mass;

		// calculate final point from force of midpoint
		// calculate velocity (position) from acceleration (velocity)
		p.position = cp_massPoints[i].position + p.velocity * timeStep;
		p.velocity = cp_massPoints[i].velocity + acceleration * timeStep;

		validatePointPosition(p);		
	}
}

// TODO maybe move to point class and use getter/setter?
void MassSpringSystemSimulator::validatePointPosition(point& p)
{
	// no validation of positions in one step test
	if (m_iTestCase != ONE_STEP_TEST) {
		Vec3 minPositions = BBOX_CENTER - BBOX_SIZE + MASS_POINT_SIZE;
		p.position.x = max(p.position.x, minPositions.x);
		p.position.y = max(p.position.y, minPositions.y);
		p.position.z = max(p.position.z, minPositions.z);

		Vec3 maxPositions = BBOX_CENTER + BBOX_SIZE - MASS_POINT_SIZE;
		p.position.x = min(p.position.x, maxPositions.x);
		p.position.y = min(p.position.y, maxPositions.y);
		p.position.z = min(p.position.z, maxPositions.z);
	}
}
