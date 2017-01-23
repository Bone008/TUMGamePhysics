#include "SphereSpringSystemSimulator.h"

SphereSpringSystemSimulator::SphereSpringSystemSimulator()
{
	// TODO testing values
	m_stiffness = 400;
	m_damping = 0.5;
	m_mass = 0.2;
	m_gravity = Vec3(0, -3, 0);
	m_gridCells = 15;
	m_gridCellCapacity = 50;

	onMouseDown = true;
	initComplete = false;
}

void SphereSpringSystemSimulator::reset()
{
}

const char * SphereSpringSystemSimulator::getTestCasesStr()
{
	return "First, Second";
}

void SphereSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	
	TwAddSeparator(DUC->g_pTweakBar, "Lucifer", "");
	TwAddVarRW(DUC->g_pTweakBar, "Damp factor", TW_TYPE_FLOAT, &m_damping, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Spring Stiffness", TW_TYPE_FLOAT, &m_stiffness, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Point Mass", TW_TYPE_FLOAT, &m_mass, "min=0.0");
	TwAddVarRW(DUC->g_pTweakBar, "Gravity force", TW_TYPE_DIR3D, &m_gravity, "");
	TwAddSeparator(DUC->g_pTweakBar, "Siegfried", "");
	TwAddVarRW(DUC->g_pTweakBar, "Grid cells", TW_TYPE_INT32, &m_gridCells, "min=1");
	TwAddVarRW(DUC->g_pTweakBar, "Cell capacity", TW_TYPE_INT32, &m_gridCellCapacity, "min=5 step=5");


	// change the camera position
	changeCameraPosition();

	switch (m_iTestCase)
	{
	case TEST_FIRST:
		break;
	case TEST_SECOND:
		break;
	}

	initComplete = true;

}

void SphereSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	reset();

	// use unique pointer for automagic memory management
	m_SphereSpringSystem = std::make_unique<SphereSpringSystem>(SphereSpringSystem(m_stiffness, m_damping, m_mass, m_gravity, m_gridCells, m_gridCellCapacity));

	switch (m_iTestCase)
	{
	case TEST_FIRST: {

		// build tower
		buildTower(Vec3(0, -BBOX_HALF_SIZE, 0), Vec3(1, BBOX_HALF_SIZE, 1));

		// FIXME this spring moves more towards point p1
		const int p0 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(-1, 2, 1), Vec3(10, -10, -10));
		const int p1 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(+1, 2, 1), Vec3(10, -10, -10));
		m_SphereSpringSystem->addSpring(p0, p1, 1.0);
		break;
	}

	case TEST_SECOND:
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

	m_SphereSpringSystem->advanceLeapFrog(timeStep,onMouseDown, mouseForce);
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
		//Mat4 viewMatrix(DUC->g_camera.GetLookAtPt());
		//Vec3 test = DUC->g_camera.GetLookAtPt();
		/*Vec3 test = Vec3(1, 0, 0);
		Vec3 color = Vec3(1, 1, 0);
		DUC->drawLine(test, color, test + 0.55f, color);*/
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

void SphereSpringSystemSimulator::buildTower(Vec3 pos, Vec3 size)
{
	for (float y = 0; y < size.y; y += 2 * SPHERE_RADIUS) {
		const int p0 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, -size.z / 2), Vec3());
		const int p1 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, +size.z / 2), Vec3());
		const int p2 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, -size.z / 2), Vec3());
		const int p3 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, +size.z / 2), Vec3());

		for (int a = p0; a < p3; a++) {
			for (int b = a + 1; b <= p3; b++) {
				m_SphereSpringSystem->addSpring(a, b);
			}
		}
	}
}

void SphereSpringSystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f, .0f, -3.5f * BBOX_HALF_SIZE);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));
}
