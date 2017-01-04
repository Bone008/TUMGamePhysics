#include "SphereSystemSimulator.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions

SphereSystemSimulator::SphereSystemSimulator()
{
	// TODO test value
	m_fRadius = 0.6f;

	// TODO test values
	addSphereSystem(NAIVEACC, Vec3(0.86f, 0.44f, 0.31f));
	addSphereSystem(GRIDACC, Vec3(0.44f, 0.86f, 0.31f));

	addSphere(Vec3(-2, 0.3f, 0), Vec3(2, 0, 0));
	addSphere(Vec3(2, 0, 0), Vec3(-2, 0, 0));
}

const char * SphereSystemSimulator::getTestCasesStr()
{
	return "Naive,Accelerated,Performance comparison";
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	// add separator
	TwAddSeparator(DUC->g_pTweakBar, "john", "");

	// for every SphereSystem: add checkbox to disable rendering
	std::string title = "Draw Sys ";
	for (int i = 0; i < m_sphereSystems.size(); i++) {
		TwAddVarRW(DUC->g_pTweakBar, (title + std::to_string(i)).c_str(), TW_TYPE_BOOLCPP, &(m_sphereSystems[i].render), "");
	}

	// change the camera position
	changeCameraPosition();

	switch (m_iTestCase)
	{
	case TEST_NAIVE:
		break;
	case TEST_ACCEL:
		break;
	case TEST_PERF_COMP:
		break;
	}
}

void SphereSystemSimulator::reset()
{
}

void SphereSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	// draw walls
	const Vec3 wallColor = Vec3(1, 1, 1);
	DUC->beginLine();
	DUC->drawLine(BBOX_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_SIZE * Vec3(-1, -1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(+1, +1, -1), wallColor, BBOX_SIZE * Vec3(+1, +1, +1), wallColor);
	
	DUC->drawLine(BBOX_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_SIZE * Vec3(+1, -1, -1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_SIZE * Vec3(+1, +1, -1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_SIZE * Vec3(-1, +1, -1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_SIZE * Vec3(+1, +1, -1), wallColor);
	
	DUC->drawLine(BBOX_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(-1, +1, +1), wallColor, BBOX_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_SIZE * Vec3(+1, -1, +1), wallColor, BBOX_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->endLine();

	// draw all SphereSystems
	for (SphereSystem& s : m_sphereSystems) {
		s.draw(DUC);
	}
}

void SphereSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	switch (m_iTestCase)
	{
	case TEST_NAIVE:
		break;
	case TEST_ACCEL:
		break;
	case TEST_PERF_COMP:
		break;
	}
}

void SphereSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	externalForce = Vec3();
}

void SphereSystemSimulator::simulateTimestep(float timeStep)
{
	// simulate for all SphereSystems
	for (SphereSystem& s : m_sphereSystems) {
		s.advanceMidPoint(timeStep);
	}
}

void SphereSystemSimulator::onClick(int x, int y)
{
}

void SphereSystemSimulator::onMouse(int x, int y)
{
}

void SphereSystemSimulator::onLeftMouseRelease()
{
}

void SphereSystemSimulator::addSphereSystem(int collisionDetectionMethod, Vec3 color)
{
	SphereSystem sSys = SphereSystem(collisionDetectionMethod, color, m_fRadius, m_fMass, m_fDamping, Vec3(0, -2, 0)); // TODO gravity
	m_sphereSystems.push_back(sSys);
}

void SphereSystemSimulator::addSphere(Vec3 pos, Vec3 vel)
{
	// all sphere system should have the same spheres
	for (SphereSystem& s : m_sphereSystems) {
		s.addSphere(pos, vel);
	}
	m_iNumSpheres++;
}

void SphereSystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f, .0f, -4 * BBOX_SIZE);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

}
