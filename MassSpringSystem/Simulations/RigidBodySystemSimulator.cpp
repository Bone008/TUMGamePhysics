#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	//UI attributes
	m_externalForce = Vec3(1, 0, 0);
	m_externalForceLocation = Vec3(0, 0, 0.5);
	m_mouse			= Point2D();
	m_trackmouse	= Point2D();
	m_oldtrackmouse = Point2D();

	initWalls();
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
			break;

		case SINGLE_BODY_SIMULATION:
			TwAddVarRW(DUC->g_pTweakBar, "External force", TW_TYPE_DIR3D, &m_externalForce, "");
			TwAddVarRW(DUC->g_pTweakBar, "External force loc", TW_TYPE_DIR3D, &m_externalForceLocation, "");
			break;
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
			addRigidBody(Vec3(0, 0, 0), Vec3(1, 0.6, 0.5), 2);
			break;

		case DOUBLE_BODY_SIMULATION:
			break;

		case COMPLEX_BODY_SIMULATION:
			Vec3 boxSize = Vec3(0.2f, 0.05f, 0.05f);
			float boxMass = 1.0f;

			buildTower(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
			buildTower(Vec3(-0.5f, -0.5f, +0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
			buildTower(Vec3(+0.5f, -0.5f, -0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
			buildTower(Vec3(+0.5f, -0.5f, +0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
			break;
	}
	
}

void RigidBodySystemSimulator::buildTower(Vec3 position, Vec3 size, Vec3 boxSize, float boxMass)
{
	float height = boxSize.y / 2;
	for (int f = 0; f < size.y / (boxSize.y * 2); f++) {
		addRigidBody(position + Vec3(0, height, +boxSize.x / 2 - boxSize.z / 2), boxSize, boxMass);
		addRigidBody(position + Vec3(0, height, -boxSize.x / 2 + boxSize.z / 2), boxSize, boxMass);
		height += boxSize.y;
		addRigidBody(position + Vec3(+boxSize.x / 2 - boxSize.z / 2, height, 0), Quat(Vec3(0, 1, 0), M_PI / 2), boxSize, boxMass);
		addRigidBody(position + Vec3(-boxSize.x / 2 + boxSize.z / 2, height, 0), Quat(Vec3(0, 1, 0), M_PI / 2), boxSize, boxMass);
		height += boxSize.y;
	}
}

void RigidBodySystemSimulator::initWalls()
{
	Vec3 size = Vec3(.001, 2 * WALL_OFFSET, 2 * WALL_OFFSET);
	//init each side
	//left
	m_walls.push_back(RigidBody(Vec3(-WALL_OFFSET, 0, 0), Quat(0, 0, 0, 1), size, 0));
	//right
	m_walls.push_back(RigidBody(Vec3(WALL_OFFSET, 0, 0), Quat(0, 0, 0, 1), size, 0));
	//back
	m_walls.push_back(RigidBody(Vec3(0, 0, WALL_OFFSET), Quat(Vec3(0, 1, 0), M_PI / 2), size, 0));
	//front
	//m_walls.push_back(RigidBody(Vec3(0, 0, -WALL_OFFSET), Quat(Vec3(0, 1, 0), M_PI / 2), size, 0));
	//top
	m_walls.push_back(RigidBody(Vec3(0, WALL_OFFSET, 0), Quat(Vec3(0, 0, 1), M_PI / 2), size, 0));
	//bottom
	m_walls.push_back(RigidBody(Vec3(0, -WALL_OFFSET, 0), Quat(Vec3(0, 0, 1), M_PI / 2), size, 0));
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
	// TODO per frame calls to RigidBody.resetExternalForces() and RigidBody.applyExternalForce(...) will go here
	
	// some testing of applying torque
	if (m_iTestCase == SINGLE_BODY_SIMULATION)
	{
		for (RigidBody& rb : m_rigidBodies)
		{
			// spin it! (but force it to stay in place)
			rb.resetExternalForces();
			rb.applyExternalForce(rb.m_objToWorldMatrix.transformVectorNormal(m_externalForce), rb.m_objToWorldMatrix.transformVector(m_externalForceLocation));
			rb.m_externalForces = Vec3();
				
			
		}
	}
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	calculateCollision();

	for (RigidBody& rb : m_rigidBodies)
		rb.integrateTimestep(timeStep);
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
	m_rigidBodies[i].applyExternalForce(force, loc);
}


void RigidBodySystemSimulator::onClick(int x, int y)
{
	onMouseDown = true;
}

//Handle the mouse press
void RigidBodySystemSimulator::onLeftMouseRelease()
{
	onMouseDown = false;
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{

}


void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	for (const RigidBody& rb : m_rigidBodies)
		rb.draw(DUC, COLOUR_RIGIDBODY);

	//draw the walls
	for (const RigidBody& w : m_walls)
		w.draw(DUC, COLOUR_WALL);

}



void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	addRigidBody(position, Quat(0, 0, 0, 1), size, mass);
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Quat orientation, Vec3 size, int mass)
{
	RigidBody rb(position, orientation, size, mass);
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

void RigidBodySystemSimulator::calculateCollision()
{
	CollisionInfo localCollisionInfo;
	for (RigidBody& a : m_rigidBodies) {
		//check with other objects
		for (RigidBody& b : m_rigidBodies) {
			//avoid duplicates
			/*if(a.m_objToWorldMatrix.isEqualTo(b.m_objToWorldMatrix))
				continue;*/

			localCollisionInfo = checkCollisionSAT(a.m_objToWorldMatrix, b.m_objToWorldMatrix);
			if (localCollisionInfo.isValid) {
				collisionInfo = localCollisionInfo;
				//TODO Handle collision between 2 objects

			}
		}
		//check with walls
		for (RigidBody& w : m_walls) {
			localCollisionInfo = checkCollisionSAT(w.m_objToWorldMatrix, a.m_objToWorldMatrix);
			if (localCollisionInfo.isValid) {
				collisionInfo = localCollisionInfo;
				//TODO Handle collision between object and wall
				
				//TODO remove this simple test
				a.m_orientation = -a.m_orientation;
				a.m_linearVelocity = -a.m_linearVelocity;
				
				//TODO Check this. I am making something wrong here :(
				/*a.m_surfaceNormal = collisionInfo.normalWorld;
				a.updateX(collisionInfo.collisionPointWorld);
				m_pRigidBodySystem.onCollisionWithWall(a);*/
			}
		}
	}
}
