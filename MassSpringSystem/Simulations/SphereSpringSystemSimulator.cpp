#include "SphereSpringSystemSimulator.h"

extern int g_iPreTestCase;

SphereSpringSystemSimulator::SphereSpringSystemSimulator()
{
	m_stiffness = 800;
	m_breakThreshold = 50;
	m_damping = 0.01;
	m_gravity = Vec3(0, -10, 0);
	m_camRotDependentGravity = false;
	m_gridCells = 6;
	m_gridCellCapacity = 256;
	m_bridgeBuilderView = false;

	onMouseDown = false;
	initComplete = false;
}

void SphereSpringSystemSimulator::reset()
{
}

float gAmazingSphereSize = 0.9;

const char * SphereSpringSystemSimulator::getTestCasesStr()
{
	return "First, Second, Third";
}

void SphereSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	
	TwAddSeparator(DUC->g_pTweakBar, "Lucifer", "");

	TwAddVarRW(DUC->g_pTweakBar, "Demolition size", TW_TYPE_FLOAT, &gAmazingSphereSize, "");
	TwAddVarRW(DUC->g_pTweakBar, "Damp factor", TW_TYPE_FLOAT, &m_damping, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Spring Stiffness", TW_TYPE_FLOAT, &m_stiffness, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Spring Threshold", TW_TYPE_FLOAT, &m_breakThreshold, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Gravity force", TW_TYPE_DIR3D, &m_gravity, "");
	TwAddVarRW(DUC->g_pTweakBar, "->Rotation Dependent", TW_TYPE_BOOLCPP, &m_camRotDependentGravity, "");

	TwAddSeparator(DUC->g_pTweakBar, "Siegfried", "");

	TwAddVarRW(DUC->g_pTweakBar, "Grid cells", TW_TYPE_INT32, &m_gridCells, "min=1");
	TwAddVarRW(DUC->g_pTweakBar, "Cell capacity", TW_TYPE_INT32, &m_gridCellCapacity, "min=5 step=5");

	TwAddSeparator(DUC->g_pTweakBar, "Shaniqua", "");

	TwAddVarRW(DUC->g_pTweakBar, "Bridge Builder View", TW_TYPE_BOOLCPP, &m_bridgeBuilderView, "");

	// change the camera position
	changeCameraPosition();

	initComplete = true;

	TwAddButton(DUC->g_pTweakBar, "Mayhem", [](void* arg) {
		auto self = (SphereSpringSystemSimulator*)arg;
		gAmazingSphereSize = 1.7;
		self->m_damping = 0.01;
		self->m_stiffness = 600;
		self->m_breakThreshold = 30;
		self->m_gravity = Vec3(0, -10, 0);

		g_iPreTestCase = -1; // reset
	}, this, "");

	TwAddButton(DUC->g_pTweakBar, "Immortal", [](void* arg) {
		auto self = (SphereSpringSystemSimulator*)arg;
		gAmazingSphereSize = 1.2;
		self->m_damping = 0.01;
		self->m_stiffness = 70;
		self->m_breakThreshold = 10000;
		self->m_gravity = Vec3(0, -7, 0);

		g_iPreTestCase = -1; // reset
	}, this, "");
	
}

void SphereSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	reset();

	// use unique pointer for automagic memory management
	m_SphereSpringSystem = std::make_unique<SphereSpringSystem>(SphereSpringSystem(m_stiffness, m_breakThreshold, m_damping, m_gravity, m_camRotDependentGravity, m_bridgeBuilderView, m_gridCells, m_gridCellCapacity));

	switch (m_iTestCase)
	{
	case TEST_FIRST: {

		// build tower
		buildTower(Vec3(0, -BBOX_HALF_SIZE, 0), Vec3(0.7, 8, 0.7));		
		//buildTower(Vec3(0, -BBOX_HALF_SIZE, 0), Vec3(1.4, BBOX_HALF_SIZE, 1.4));
		//buildTower(0.75*Vec3(-1, -BBOX_HALF_SIZE, -1), Vec3(0.7, BBOX_HALF_SIZE, 0.7));
		//buildTower(0.75*Vec3(-1, -BBOX_HALF_SIZE, 1), Vec3(0.7, BBOX_HALF_SIZE, 0.7));
		//buildTower(0.75*Vec3(1, -BBOX_HALF_SIZE, 1), Vec3(0.7, BBOX_HALF_SIZE, 0.7));
		//buildTower(0.75*Vec3(1, -BBOX_HALF_SIZE, -1), Vec3(0.7, BBOX_HALF_SIZE, 0.7));

		//const int p0 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(-1, 2, 1), Vec3(10, -10, -10));
		//const int p1 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(+1, 2, 1), Vec3(10, -10, -10));
		//m_SphereSpringSystem->addSpring(p0, p1, 1.0);

		m_SphereSpringSystem->addSphere(Vec3(-4.5, 0, 0), 0.5*Vec3(15, 0, 0), gAmazingSphereSize);
		m_SphereSpringSystem->addSphere(Vec3(4.5, -2.5, 0), 0.6*Vec3(-15, 0, 0), gAmazingSphereSize);
		//m_SphereSpringSystem->addSphere(Vec3(4.5, -1, 0), 0.2*Vec3(-25, 1, 0), 0.3);
		//m_SphereSpringSystem->addSphere(Vec3(0, 4, 0), Vec3(0, -1, 0), gAmazingSphereSize);
		break;
	}

	case TEST_THIRD:
		buildBuilding(Vec3(0, -5.5, 0), Vec3(11, 2.5, 11), Vec3(15, 4, 15), 0.35, false);

		m_SphereSpringSystem->addSphere(Vec3(1, 4, 0), Vec3(0, 0, 0), gAmazingSphereSize);

		break;
	}
}

void SphereSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void SphereSpringSystemSimulator::simulateTimestep(float timeStep)
{
	//it is substract by 10 to be more realistic and not that expensive
	Vec3 mouseForce = (m_mouseLocalCoordinate - m_mouseOldLocalCoordinate) * 10;

	m_SphereSpringSystem->advanceLeapFrog(timeStep, DUC, onMouseDown, mouseForce);
}

void SphereSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	// draw walls
	const Vec3 wallColor = Vec3(1, 1, 1);
	DUC->beginLine();
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);

	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor);

	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->endLine();

	// draw sphere spring system
	m_SphereSpringSystem->draw(DUC);
}

void SphereSpringSystemSimulator::onClick(int x, int y)
{
	//if it is the first time just update the mouse vector
	if (!onMouseDown) {
		//update the mouse position
		m_mouse = Vec3(x, y, 0);
	}
	else {
		m_oldtrackmouse = m_mouse;
		//update the mouse position
		m_mouse = Vec3(x, y, 0);
		//update the local mouse position with center on the screen at 0,0,0
		m_mouseLocalCoordinate = toLocalCoordinate(m_mouse);

		//update the old local mouse position with center on the screen at 0,0,0
		m_mouseOldLocalCoordinate = toLocalCoordinate(m_oldtrackmouse);
	}

	//update the boolean
	onMouseDown = true;
}

void SphereSpringSystemSimulator::onLeftMouseRelease()
{
	onMouseDown = false;

	//reset mouse positions
	m_mouse = Vec3();
	m_oldtrackmouse = Vec3();
	m_mouseOldLocalCoordinate = Vec3();
	m_mouseLocalCoordinate = Vec3();
}

void SphereSpringSystemSimulator::onMouse(int x, int y)
{
	if (initComplete) {
		Vec3 toLocal = toLocalCoordinate(Vec3(x, y, 0));
		float norm_x = (((2.0f * toLocal.x) / m_screenWidth));
		float norm_y = (((2.0f * toLocal.y) / m_screenHeight));
		float scallingFactor = 6.f;
		Vec3 normPos = Vec3(norm_x*scallingFactor, norm_y*scallingFactor, CAMERA_POSITION_Z + 0.5f);
		m_mouseShootingPosition = normPos;
		cout << m_mouseShootingPosition << endl;
	}
}

void SphereSpringSystemSimulator::onKeyboardSpaceDown()
{
	if (initComplete) {
		int p0,p1;
		switch (m_iTestCase)
		{
		case TEST_FIRST:
			p0 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition + SHOOT_SPHERE_SPRING_DIMENSION, SHOOT_SPHERE_SPRING_VELOCITY, 0.5);
			p1 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition - SHOOT_SPHERE_SPRING_DIMENSION, SHOOT_SPHERE_SPRING_VELOCITY, 0.5);
			m_SphereSpringSystem->addSpring(p0, p1, 1.0);
			cout << "Shooting\n";
			break;
		case TEST_SECOND:
			//TODO i will remove it when add my scene :)
			p0 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition + SHOOT_SPHERE_SPRING_DIMENSION, SHOOT_SPHERE_SPRING_VELOCITY, 0.5);
			p1 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition - SHOOT_SPHERE_SPRING_DIMENSION, SHOOT_SPHERE_SPRING_VELOCITY, 0.5);
			m_SphereSpringSystem->addSpring(p0, p1, 1.0);
			cout << "Shooting\n";
			break;
		}
		/*int p0 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition + Vec3(0.5, 0, 0), Vec3(0, 0, 1000));
		int p1 = m_SphereSpringSystem->addSphere(m_mouseShootingPosition - Vec3(0.5, 0, 0), Vec3(0, 0, 1000));
		m_SphereSpringSystem->addSpring(p0, p1, 1.0);*/
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
Vec3 SphereSpringSystemSimulator::toLocalCoordinate(Vec3 globalScreenPosition)
{
	return Vec3(globalScreenPosition.x - m_screenWidth / 2, m_screenHeight - globalScreenPosition.y - m_screenHeight / 2, globalScreenPosition.z);
}

Vec3 SphereSpringSystemSimulator::get3dPosFrom1dIndex(int index, Vec3 maxs) {
	// turns the index into its x/y/z coordinates
	int tmp = index;
	const int y = tmp / (maxs.x * maxs.z);
	tmp -= y * (maxs.x * maxs.z);
	const int x = tmp / maxs.z;
	const int z = tmp % (int)maxs.z;

	return Vec3(x, y, z);
}

void SphereSpringSystemSimulator::buildBuilding(Vec3 basePos, Vec3 size, Vec3 numSpheres, float sphereRadius, bool basementFixed, bool faceDiagonals, bool bodyDiagonals)
{
	basePos += Vec3(0, sphereRadius + 0.01, 0);
	
	// length of the springs in every dimension
	const Vec3 springLengths = size / (numSpheres - Vec3(1));

	// position of the corner sphere
	const Vec3 minBasePos = basePos - Vec3(size.x / 2, 0, size.z / 2);
	
	// for every floor, save the last sphere index
	int indLast = -1;
	for (int y = 0; y < numSpheres.y; y++) {
		// only lowest floor is fixed, if param basementFixed is true
		const bool fixed = basementFixed && y == 0;

		// create spheres for the whole floor, remember index of last one
		int lastIndex = -1;
		for (int x = 0; x < numSpheres.x; x++) {
			for (int z = 0; z < numSpheres.z; z++) {
				const Vec3 pos = minBasePos + Vec3(x, y, z) * springLengths;
				indLast = m_SphereSpringSystem->addSphere(pos, Vec3(), sphereRadius, fixed);
			}
		}
	}

	const int numTotalSpheres = numSpheres.x * numSpheres.y * numSpheres.z;
	const int indFirst = indLast - numTotalSpheres + 1;

	// for every added sphere
	for (int index1 = indFirst; index1 <= indLast; index1++) {
		// get pos of that sphere
		Vec3 pos1 = get3dPosFrom1dIndex(index1 - indFirst, numSpheres);

		// for every other added sphere
		for (int index2 = index1 + 1; index2 <= indLast; index2++) {
			// get pos again
			Vec3 pos2 = get3dPosFrom1dIndex(index2 - indFirst, numSpheres);

			// the absolute differences between both positions x/y/z
			Vec3 absOffset = (pos1 - pos2).getAbsolutes();

			// the Vec3 method didnt work...
			int maxComponent = max(absOffset.x, max(absOffset.y, absOffset.z));

			// we only want (diagonally/directly) adjacent spheres
			if (maxComponent == 1) {
				int componentSum = absOffset.x + absOffset.y + absOffset.z;

				// componentSum:
				// + 1: offset in 1 dimension  -> directly adjacent
				// + 2: offset in 2 dimensions -> face diagonals
				// + 3: offset in 3 dimensions -> body diagonals
				if (componentSum == 1
					|| componentSum == 2 && faceDiagonals
					|| componentSum == 3 && bodyDiagonals) {
					m_SphereSpringSystem->addSpring(index1, index2);
				}
			}
		}
	}
}

void SphereSpringSystemSimulator::buildTower(Vec3 pos, Vec3 size)
{
	const float sradius = 0.2;
	pos += Vec3(0, sradius + 0.01, 0);

 	for (float y = 0; y < size.y; y += 3.5 * sradius) {
		const int p0 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, -size.z / 2), Vec3(), sradius, y == 0);
		const int p1 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, +size.z / 2), Vec3(), sradius, y == 0);
		const int p2 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, +size.z / 2), Vec3(), sradius, y == 0);
		const int p3 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, -size.z / 2), Vec3(), sradius, y == 0);

		for (int a = p0; a <= p3; a++) {
			for (int b = a + 1; b <= p3; b++) {
				m_SphereSpringSystem->addSpring(a, b);
			}

			if (y > 0)
			{
				m_SphereSpringSystem->addSpring(a, p0 - 4);
				m_SphereSpringSystem->addSpring(a, p0 - 3);
				m_SphereSpringSystem->addSpring(a, p0 - 2);
				m_SphereSpringSystem->addSpring(a, p0 - 1);
				//m_SphereSpringSystem->addSpring(a, p0 - 4 + ((a - p0 + 2) % 4));
			}
		}
	}
}

void SphereSpringSystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f, .0f, CAMERA_POSITION_Z);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));
}
