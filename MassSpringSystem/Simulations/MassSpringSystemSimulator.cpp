#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = TWO_POINT_SETUP;

	// Data Attributes
	m_fMass       = 10;
	m_fStiffness  = 40;
	m_fDamping    = 1.0f;
	m_iIntegrator = MIDPOINT;

	// UI Attributes
	m_externalForce = Vec3();
	m_mouseForce	= Vec3();
	m_mouse         = Point2D();
	m_trackmouse    = Point2D();
	m_oldtrackmouse = Point2D();
	m_collision		= true; // TODO important to set to false for the automatic tests

	firstTime = true; // important for the leap-frog 

	m_mouseInteraction = true; // twakbar boolean

	isMouseActive = false; //measures whether the mouse is active or not


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
		TwAddVarRW(DUC->g_pTweakBar, "Spring Stiffness", TW_TYPE_FLOAT, &this->m_fStiffness, "min=0.0");
		TwAddVarRW(DUC->g_pTweakBar, "Point Mass", TW_TYPE_FLOAT, &this->m_fMass, "min=0.0");
		TwAddVarRW(DUC->g_pTweakBar, "Gravity force", TW_TYPE_DIR3D, &this->m_externalForce, "");
		TwAddVarRW(DUC->g_pTweakBar, "Collision", TW_TYPE_BOOLCPP, &this->m_collision, "");		
		TwAddVarRW(DUC->g_pTweakBar, "Mouse interaction", TW_TYPE_BOOLCPP, &this->m_mouseInteraction, "");
		TwAddVarRW(DUC->g_pTweakBar, "Mouse force", TW_TYPE_DIR3D, &this->m_mouseForce, "");
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
			const float size = 0.1f;

			// vertices
			int p000 = addMassPoint(pos + Vec3(0, 0, 0), Vec3(), false);
			int p001 = addMassPoint(pos + Vec3(0, 0, size), Vec3(), false);
			int p010 = addMassPoint(pos + Vec3(0, size, 0), Vec3(), false);
			int p011 = addMassPoint(pos + Vec3(0, size, size), Vec3(), false);
			int p100 = addMassPoint(pos + Vec3(size, 0, 0), Vec3(), false);
			int p101 = addMassPoint(pos + Vec3(size, 0, size), Vec3(), false);
			int p110 = addMassPoint(pos + Vec3(size, size, 0), Vec3(), false);
			int p111 = addMassPoint(pos + Vec3(size, size, size), Vec3(), false);

			const float springLengthFac = size;

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
			const float halfSize = 0.2f;
			const Vec3 pos = Vec3(0, 0.5f - halfSize, 0);

			// 4 corners
			int a = addMassPoint(pos + Vec3(+halfSize, +halfSize, -halfSize), Vec3(), true);
			int b = addMassPoint(pos + Vec3(-halfSize, -halfSize, -halfSize), Vec3(), false);
			int c = addMassPoint(pos + Vec3(-halfSize, +halfSize, +halfSize), Vec3(), true);
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
		applyMouseForce(timeStep);
		integrate(timeStep);
		break;		
	}
}

// on left click
void MassSpringSystemSimulator::onClick(int x, int y)
{
	if (m_mouseInteraction) {
	
		isMouseActive = true;

		//check if old mouse was set
		if (!isSet(m_mouse)) {
			m_mouse = setBoth(x, y);
		}
		else {
			m_trackmouse = setBoth(x, y);
			//try to ignore just random clicks on the screen
			if (getDistance(m_trackmouse, m_oldtrackmouse) > MAX_MOUSE_DISTANCE && isSet(m_oldtrackmouse)) {
				m_oldtrackmouse = m_mouse;
				m_mouse = m_trackmouse;
			}
			else {
				//it is real mouse input so handle it
				m_oldtrackmouse = m_mouse;
				m_mouse = m_trackmouse;
				Vec3 direction = getMouseDirection(m_mouse, m_oldtrackmouse);
				m_mouseForce += direction;
			}
		}
	} 
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

Point2D MassSpringSystemSimulator::setBoth(int x, int y)
{
	Point2D res = Point2D();
	res.x = x;
	res.y = y;
	return res;
}
// checks if the point is is set
bool MassSpringSystemSimulator::isSet(Point2D p)
{
	return p.x != 0 && p.y != 0;
}


Vec3 MassSpringSystemSimulator::getMouseDirection(Point2D mouse, Point2D oldMouse)
{
	Vec3 dir = Vec3(0,0,0);

	dir.y = oldMouse.y - mouse.y;
	
	dir.x = mouse.x - oldMouse.x;

	//small corection because its too big value
	return dir/10;

}

int MassSpringSystemSimulator::getDistance(Point2D a, Point2D b)
{
	return abs(a.x - b.x) >= abs(a.y - b.y) ? abs(a.x - b.x) : abs(a.y - b.y);
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
		currentSpring.computeElasticForces(m_fDamping);
	}
}

void MassSpringSystemSimulator::integrate(float timeStep)
{
	switch (m_iIntegrator)
	{
	case EULER:
		integrateEuler(timeStep);
		//reset the leapfrog boolean
		firstTime = true;
		break;

	case MIDPOINT:
		integrateMidpoint(timeStep);
		//reset the leapfrog boolean
		firstTime = true;
		break;

	case LEAPFROG:
		integrateLeapfrog(timeStep);
		break;
	}

	// collision
	// no validation of positions in one step test
	if (m_collision && m_iTestCase != ONE_STEP_TEST)			
	{
		for(point& p : m_massPoints) 
		{
			validatePointPositionAndMouseIntegration(p);
		}
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
	}
}

void MassSpringSystemSimulator::integrateMidpoint(float timeStep)
{
	// create copy of masspoints
	std::vector<point> cp_massPoints(m_massPoints);
	
	// first step: calculate midpoint values
	integrateEuler(timeStep / 2);

	// recompute elastic forces based on midpoint
	clearForces();
	computeElasticForces();
	applyExternalForce(m_externalForce);
	applyMouseForce(timeStep);

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
	}

	//reset the leapfrog boolean
	firstTime = true;
}

void MassSpringSystemSimulator::integrateLeapfrog(float timeStep)
{
	//First time the object velocity should be at midpoint
	if (firstTime) {
		for (point& p : m_massPoints)
		{
			// ignore fixed points
			if (p.isFixed)
				continue;

			// calculate acceleration a = f/m
			Vec3 acceleration = p.force / p.mass;

			// calculate velocity from acceleration and delta t /2 
			p.velocity += acceleration * timeStep/2;

			// calculate position from velocity and delta t
			p.position += p.velocity * timeStep;

		}
	}
	else {
		for (point& p : m_massPoints)
		{
			// ignore fixed points
			if (p.isFixed)
				continue;

			// calculate acceleration a = f/m
			Vec3 acceleration = p.force / p.mass;

			// calculate velocity from acceleration and delta t 
			p.velocity += acceleration * timeStep;

			// calculate position from velocity and delta t
			p.position += p.velocity * timeStep;
		}
	}
}

void MassSpringSystemSimulator::validatePointPositionAndMouseIntegration(point& p)
{
	Vec3 minPositions = BBOX_CENTER - BBOX_SIZE + MASS_POINT_SIZE;
	Vec3 maxPositions = BBOX_CENTER + BBOX_SIZE - MASS_POINT_SIZE;

	bool isAtTheBottom = minPositions.y > p.position.y || minPositions.x > p.position.x;
	bool isAtTheTop = maxPositions.y < p.position.y || maxPositions.x < p.position.x;
	if (isAtTheBottom || isAtTheTop){
		//if the mouse is not active just change the position of the point
		if (!isMouseActive) {
			//min position
			if (p.position.x < minPositions.x) {
				p.position.x = minPositions.x;
				p.velocity.x = 0;
			}
			if (p.position.y < minPositions.y) {
				p.position.y = minPositions.y;
				p.velocity.y = 0;
			}
			if (p.position.z < minPositions.z) {
				p.position.z = minPositions.z;
				p.velocity.z = 0;
			}

			//max position
			if (p.position.x > maxPositions.x) {
				p.position.x = maxPositions.x;
				p.velocity.x = 0;
			}
			if (p.position.y > maxPositions.y) {
				p.position.y = maxPositions.y;
				p.velocity.y = 0;
			}
			if (p.position.z > maxPositions.z) {
				p.position.z = maxPositions.z;
				p.velocity.z = 0;
			}

			//always reset the mouse force
			clearMouseForce();
		}else{
			//apply the mouse force when needed
			if (m_mouseForce.y > m_externalForce.y) {
				p.clearForce();
				//apply only here the mouse force directly to the velocity
				//this can lead to calculation mistake but in order to work
				//as natural as posible should be left like this!
				//p.force += m_mouseForce isn't realistic at all!!
				p.velocity = m_mouseForce;
				isMouseActive = false;
			}
			else {
				//otherwise just clean it 
				clearMouseForce();
				isMouseActive = false;
			}

		}
	}
}

void MassSpringSystemSimulator::applyMouseForce(float timeStep)
{
	//set limit
	if (m_mouseForce > MAX_MOUSE_FORCE)
		m_mouseForce = MAX_MOUSE_FORCE;

	//for more realistic effect add the gravity
	if (m_mouseForce.y != 0) {
		m_mouseForce += m_externalForce*timeStep;

	}

	for (point& p : m_massPoints)
	{
		p.force += m_mouseForce;
	}
}

void MassSpringSystemSimulator::clearMouseForce()
{
	m_mouseForce = 0;
}

