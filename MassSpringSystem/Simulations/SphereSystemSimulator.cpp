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
	m_fRadius = 0.6f;
	m_fMass = 1.0f;
	m_fDamping = 0.2f;
	m_gravity = Vec3(0, -3, 0);
	m_iNumSpheres = 100;
	m_useNaive = true;
	m_useGrid = true;

	m_iGridCells = 4;
	m_iGridCapacity = 50;

	m_iKernel = 1; // linear kernel, default from formula

	m_camRotDependentGravity = false;
}

const char * SphereSystemSimulator::getTestCasesStr()
{
	return "Simple,Complex,Performance comparison";
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	// add separator
	TwAddSeparator(DUC->g_pTweakBar, "john", "");

	TwAddVarRW(DUC->g_pTweakBar, "Spheres", TW_TYPE_INT32, &m_iNumSpheres, "");
	TwAddVarRW(DUC->g_pTweakBar, "Radius", TW_TYPE_FLOAT, &m_fRadius, "");
	TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "");
	TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "");
	TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_DIR3D, &m_gravity, "");
	TwAddVarRW(DUC->g_pTweakBar, "->Rotation Dependent", TW_TYPE_BOOLCPP, &m_camRotDependentGravity, "");

	if (m_useGrid)
	{
		TwAddVarRW(DUC->g_pTweakBar, "Grid cells", TW_TYPE_INT32, &m_iGridCells, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "Cell capacity", TW_TYPE_INT32, &m_iGridCapacity, "min=1");
	}

	TwAddSeparator(DUC->g_pTweakBar, "siegfried", "");

	TwAddVarRW(DUC->g_pTweakBar, "Compute simple", TW_TYPE_BOOLCPP, &m_useNaive, "");
	TwAddVarRW(DUC->g_pTweakBar, "Compute uniform grid", TW_TYPE_BOOLCPP, &m_useGrid, "");

	TwAddSeparator(DUC->g_pTweakBar, "bert", "");

	// for every SphereSystem: add checkbox to disable rendering
	std::string title = "Draw Sys ";
	for (int i = 0; i < m_sphereSystems.size(); i++) {
		TwAddVarRW(DUC->g_pTweakBar, (title + std::to_string(i)).c_str(), TW_TYPE_BOOLCPP, &(m_sphereSystems[i].render), "");
	}

	TwAddSeparator(DUC->g_pTweakBar, "despair", "");

	// set kernels as enum for selection field
	TwEnumVal kernelEnum[] = { {0, "Constant"}, {1, "Linear"}, {2, "Quadratic"}, {3, "Weak Electric Charge"}, {4, "Electric Charge"} };
	TwType kernelType = TwDefineEnum("Ingeborg", kernelEnum, 5);
	TwAddVarRW(DUC->g_pTweakBar, "Kernel", kernelType, &m_iKernel, "");

	// change the camera position
	changeCameraPosition();

	switch (m_iTestCase)
	{
	case TEST_SIMPLE:
		break;
	case TEST_COMPLEX:
		break;
	case TEST_PERF_COMP:
		break;
	}
}

void SphereSystemSimulator::reset()
{
	m_sphereSystems.clear();
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

float randFloat(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

void SphereSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	reset();

	if (m_useNaive) addSphereSystem(NAIVEACC, Vec3(0.86f, 0.44f, 0.31f));
	if (m_useGrid)  addSphereSystem(GRIDACC, Vec3(0.44f, 0.86f, 0.31f));

	switch (m_iTestCase)
	{
	case TEST_SIMPLE:
		addSphere(Vec3(-2, 0.3f, 0), Vec3(2, 0, 0));
		addSphere(Vec3(2, 0, 0), Vec3(-2, 0, 0));
		break;
	case TEST_COMPLEX:
		for (int i = 0; i < m_iNumSpheres; i++)
		{
			float r = BBOX_SIZE - 1;
			Vec3 pos(randFloat(-r, r), randFloat(-r, r), randFloat(-r, r));
			Vec3 vel(randFloat(-r, r), randFloat(-r, r), randFloat(-r, r));
			addSphere(pos, vel);
		}
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
		s.advanceLeapfrog(timeStep, DUC);
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
	// push_back, but with more magic and less copy
	m_sphereSystems.emplace_back(collisionDetectionMethod, color, m_fRadius, m_fMass, m_fDamping, m_gravity, m_iGridCells, m_iGridCapacity, m_Kernels[m_iKernel], m_camRotDependentGravity);
	
	//m_sphereSystems.push_back(SphereSystem(collisionDetectionMethod, color, m_fRadius, m_fMass, m_fDamping, m_gravity));
}

void SphereSystemSimulator::addSphere(Vec3 pos, Vec3 vel)
{
	// all sphere system should have the same spheres
	for (SphereSystem& s : m_sphereSystems) {
		s.addSphere(pos, vel);
	}
}

void SphereSystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f, .0f, -4 * BBOX_SIZE);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

}
