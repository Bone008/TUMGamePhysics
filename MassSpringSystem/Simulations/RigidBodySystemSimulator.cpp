#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	//UI attributes
	m_externalForce				= Vec3(1, 0, 0);
	m_externalForceLocation		= Vec3(0, 0, 0.5);
	m_mouse						= Vec3();
	m_mouseLocalCoordinate		= Vec3();
	m_mouseOldLocalCoordinate	= Vec3();
	m_oldtrackmouse				= Vec3();

	wallOffset = 0;

	onMouseDown = false;

}

// Functions
const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "One-step test, Single rigid body, Two-rigid-body, Complex Simulation";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	resetWalls();
	switch (m_iTestCase)
	{
		case ONE_STEP_SIMULATION: // no ui needed
			//draw custom walls and custom camera for each setup
			wallOffset = 2.0f;
			initWalls();
			changeCameraPosition();
			break;

		case SINGLE_BODY_SIMULATION:
			TwAddVarRW(DUC->g_pTweakBar, "External force", TW_TYPE_DIR3D, &m_externalForce, "");
			TwAddVarRW(DUC->g_pTweakBar, "External force loc", TW_TYPE_DIR3D, &m_externalForceLocation, "");
			//draw custom walls and custom camera for each setup
			wallOffset = 4.0f;
			initWalls();
			changeCameraPosition();

			break;
		case DOUBLE_BODY_SIMULATION:
		case COMPLEX_BODY_SIMULATION:
			TwAddVarRW(DUC->g_pTweakBar, "External force", TW_TYPE_DIR3D, &m_externalForce, "");
			TwAddVarRW(DUC->g_pTweakBar, "External force loc", TW_TYPE_DIR3D, &m_externalForceLocation, "");
			//draw custom walls and custom camera for each setup
			wallOffset = 6.0f;
			initWalls();
			changeCameraPosition();
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
			cout << getNumberOfRigidBodies() << endl;
			break;

		case DOUBLE_BODY_SIMULATION:
			addRigidBody(Vec3(1, 2, 0), Vec3(1, .7, .2), 2);
			applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(-1, -1, 0));
			addRigidBody(Vec3(-2, -1, 0), Vec3(1, .5, 0.5), 2);
			applyForceOnBody(1, Vec3(0.3, 0.5, 0.25), Vec3(1, 1, 0));
			break;

		case COMPLEX_BODY_SIMULATION:
			// add 7 beautiful blocks in a row
			for (int i = -3; i <= 3; i++) {
				addRigidBody(Vec3(i , 0, 0), Vec3(0.2, 2.0, 0.5), 1);
			}

			// destroy them
			addRigidBody(Vec3(5, 1, -5), Vec3(2, 1, 1), 1);
			int rIndex = m_rigidBodies.size() - 1;
			applyForceOnBody(rIndex, getPositionOfRigidBody(rIndex), Vec3(-200, 0, 200));

			/*
				//isn't efficient with external forces

				Vec3 boxSize = Vec3(0.2f, 0.05f, 0.05f);
				float boxMass = 1.0f;

				buildTower(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
				buildTower(Vec3(-0.5f, -0.5f, +0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
				buildTower(Vec3(+0.5f, -0.5f, -0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
				buildTower(Vec3(+0.5f, -0.5f, +0.5f), Vec3(0.2f, 1.0f, 0.2f), boxSize, boxMass);
			*/
			
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
	Vec3 size = Vec3(.001, 2 * wallOffset, 2 * wallOffset);
	//init each side
	//left
	m_walls.push_back(RigidBody(Vec3(-wallOffset, 0, 0), Quat(0, 0, 0, 1), size, 0));
	//right
	m_walls.push_back(RigidBody(Vec3(wallOffset, 0, 0), Quat(0, 0, 0, 1), size, 0));
	//back
	m_walls.push_back(RigidBody(Vec3(0, 0, wallOffset), Quat(Vec3(0, 1, 0), M_PI / 2), size, 0));
	//front
	//m_walls.push_back(RigidBody(Vec3(0, 0, -wallOffset), Quat(Vec3(0, 1, 0), M_PI / 2), size, 0));
	//top
	m_walls.push_back(RigidBody(Vec3(0, wallOffset, 0), Quat(Vec3(0, 0, 1), M_PI / 2), size, 0));
	//bottom
	m_walls.push_back(RigidBody(Vec3(0, -wallOffset, 0), Quat(Vec3(0, 0, 1), M_PI / 2), size, 0));
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
	//if it is the first time just update the mouse vector
	if (!onMouseDown) {
		//update the mouse position
		m_mouse = Vec3(x, y, 0);
	}else{
		m_oldtrackmouse = m_mouse;
		//update the mouse position
		m_mouse = Vec3(x, y, 0);
		//update the local mouse position with center on the screen at 0,0,0
		m_mouseLocalCoordinate = toLocalCoordinate(m_mouse);
		//update the old local mouse position with center on the screen at 0,0,0
		m_mouseOldLocalCoordinate = toLocalCoordinate(m_oldtrackmouse);

		//it is substract by 10 to be more realistic and not that expensive
		Vec3 mouseForce = (m_mouseLocalCoordinate- m_mouseOldLocalCoordinate)/10;

		//apply it to all bodies
		applyForceOnEachBody(mouseForce);
	}

	//update the boolean
	onMouseDown = true;
}

//Handle the mouse press
void RigidBodySystemSimulator::onLeftMouseRelease()
{
	onMouseDown = false;

	//reset mouse positions
	m_mouse						= Vec3();
	m_oldtrackmouse				= Vec3();
	m_mouseOldLocalCoordinate	= Vec3();
	m_mouseLocalCoordinate		= Vec3();
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
}


void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	for (const RigidBody& rb : m_rigidBodies) {
		rb.draw(DUC, COLOUR_RIGIDBODY);
		//draw the mouse spring
		if (onMouseDown) {
			DUC->beginLine();
			DUC->drawLine(rb.m_position, COLOUR_MOUSE_VECTOR, (m_mouseLocalCoordinate / MOUSE_VECTOR_LENGTH_SUBTRACTOR), COLOUR_MOUSE_VECTOR);
			DUC->endLine();
		}
	}
		
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
		// TODO both loops have almost the same body. somehow unite?
		//check with other rigid bodies
		for (RigidBody& b : m_rigidBodies) {
			//avoid duplicates
			if (&a == &b)
				continue;

			localCollisionInfo = checkCollisionSAT(a.m_objToWorldMatrix, b.m_objToWorldMatrix);
			if (localCollisionInfo.isValid) {
				// vRel
				Vec3 vRel = a.m_linearVelocity - b.m_linearVelocity;

				// vRel * n
				double separationIndicator = dot(vRel, localCollisionInfo.normalWorld);

				// ignore separating bodies
				if (separationIndicator > 0) {
					continue;
				}

				// store surface normal
				// TODO ackchyually only one copy of this needs to be stored
				a.m_surfaceNormal = localCollisionInfo.normalWorld;
				b.m_surfaceNormal = localCollisionInfo.normalWorld;

				// handle collision
				m_pRigidBodySystem->onCollision(a, b, localCollisionInfo.collisionPointWorld, false);
			}
		}

		//check with walls
		for (RigidBody& w : m_walls) {
			localCollisionInfo = checkCollisionSAT(a.m_objToWorldMatrix, w.m_objToWorldMatrix);
			if (localCollisionInfo.isValid) {
				// as walls are not moving, vRel is just velocity of a
				// vRel * n
				double separationIndicator = dot(a.m_linearVelocity, localCollisionInfo.normalWorld);

				// ignore separating bodies
				if (separationIndicator > 0) {
					continue;
				}

				// store surface normal
				a.m_surfaceNormal = localCollisionInfo.normalWorld;
				w.m_surfaceNormal = localCollisionInfo.normalWorld;

				// handle collision
				m_pRigidBodySystem->onCollision(a, w, localCollisionInfo.collisionPointWorld, true);
			}
		}
	}
}

/*
As we have the screen of the center at [0,0] then we have to change the 'x' and 'y' coordinates of the mouse:
	'x' start from left to right and it global pixel positions are between [0,maxWidth]
	and it should be between [-maxWidth/2, +maxWidth/2] thus the center is at 0

	'y' start from the top (0) and it global pixel positions are between [0,maxHeight] 
	where maxHeight points the bottom of the screen. We need it to be 
	between [+maxHeight,-maxHeight] where 0 is the center.

Here we make those changes of the 'x' and 'y' component of the mouse
*/
Vec3 RigidBodySystemSimulator::toLocalCoordinate(Vec3 globalScreenPosition)
{
	return Vec3(globalScreenPosition.x - m_screenWidth/2, m_screenHeight - globalScreenPosition.y - m_screenHeight/2, globalScreenPosition.z);
}

//TODO Reset the mouse input force
void RigidBodySystemSimulator::applyForceOnEachBody(Vec3 force)
{
	for (auto i = 0; i < getNumberOfRigidBodies(); i++)
		applyForceOnBody(i, getPositionOfRigidBody(i), force);

}

void RigidBodySystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f,.0f, -4*wallOffset);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));
}

void RigidBodySystemSimulator::resetWalls()
{
	m_walls.clear();
}


