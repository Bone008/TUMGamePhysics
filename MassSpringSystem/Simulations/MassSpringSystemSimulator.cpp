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
		drawMassPoint(tmpPoint);
	}

	// draw all springs
	for each(spring tmpSpring in m_springs)
	{
		drawSpring(tmpSpring);
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
		setIntegrator(EULER);

		addSpring(
			addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false),
			addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false),
			1
		);		

		break;

	case 3:
		cout << "3: Simple Midpoint simulation!\n";
		break;

	case 4:
		cout << "4: Complex simulation, stability comparison!\n";
		break;

	case 5:
		cout << "5: Leap-Frog method!\n";
		break;

	default:
		cout << "Empty test!\n";
		notifyCaseChanged(2); // just for testing purpose TODO remove
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

// Drawing functions
void MassSpringSystemSimulator::drawMassPoint(point p)
{
	DUC->setUpLighting(Vec3(), MASS_POINT_COLOR, 100, MASS_POINT_COLOR);
	DUC->drawSphere(p.position, Vec3(MASS_POINT_SIZE, MASS_POINT_SIZE, MASS_POINT_SIZE));
}

void MassSpringSystemSimulator::drawSpring(spring s)
{
	// get start/end points of spring
	point p1 = m_massPoints.at(s.point1);
	point p2 = m_massPoints.at(s.point2);	

	// draw line between the two points
	DUC->beginLine();
	DUC->drawLine(p1.position, SPRING_COLOR, p2.position, SPRING_COLOR);
	DUC->endLine();
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

// TODO move to spring-class!
void MassSpringSystemSimulator::computeElasticForces()
{
	// calculate forces of each point
	for (spring currentSpring : m_springs)
	{
		// shortcut to endpoints of spring
		point* p1 = &m_massPoints.at(currentSpring.point1);
		point* p2 = &m_massPoints.at(currentSpring.point2);

		// calculate current length of spring
		float currentLength = sqrt(p1->position.squaredDistanceTo(p2->position));

		// calculate force of spring/p1
		Vec3 force = -m_fStiffness
			* (currentLength - currentSpring.initialLength)
			* ((p1->position - p2->position) / currentLength);

		// store forces in points for later integration
		p1->force += force;
		p2->force -= force;
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
