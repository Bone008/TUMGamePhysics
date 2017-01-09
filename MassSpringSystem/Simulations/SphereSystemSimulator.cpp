﻿#include "SphereSystemSimulator.h"

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
	m_fMass = 1.0f;
	m_fDamping = 0.5f;

	// TODO test values
	addSphereSystem(NAIVEACC, Vec3(0.86f, 0.44f, 0.31f));
	addSphereSystem(GRIDACC, Vec3(0.44f, 0.86f, 0.31f));

	//aways start the sphere index from 1!
	addSphere(1, Vec3(-2, 0.3f, 0), Vec3(2, 0, 0));
	addSphere(2, Vec3(2, 0, 0), Vec3(-2, 0, 0));

	getScreenResolution();	//get here the resolution before the init of directX 
	//lower the resolution in order to have smaller size of uniform grid array
	//this is made for the 3D in order to save memory
	float smallerScreenWidth = BBOX_SIZE*m_screenWidth / RESOLUTION_CORRECTOR;
	float smallerScreenHeight = BBOX_SIZE*m_screenHeight / RESOLUTION_CORRECTOR;
	float smallerScreenDepth = BBOX_SIZE*RESOLUTION_CORRECTOR;
	
	//init the uniform grid class
	uniformGrid = UniformGridCollision(2 * m_fRadius, m_sphereSystems.size(), smallerScreenWidth, smallerScreenHeight, smallerScreenDepth);

	onMouseDown = false;
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

		for (SphereSystem::Sphere& sp : s.getSpheres()) {
			//draw the mouse spring
			if (onMouseDown) {
				DUC->beginLine();
				DUC->drawLine(sp.pos, COLOUR_MOUSE_VECTOR, (m_mouseLocalCoordinate / MOUSE_VECTOR_LENGTH_SUBTRACTOR), COLOUR_MOUSE_VECTOR);
				DUC->endLine();
			}
		}
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
		//apply the mouse force
		if (onMouseDown) {
			//it is substract by 10 to be more realistic and not that expensive
			Vec3 mouseForce = (m_mouseLocalCoordinate - m_mouseOldLocalCoordinate) * 10;
			//apply it to each sphere
			s.UpdateVelocities(timeStep, mouseForce);
		}
			
	}
	//as every sphere system holds all of the spheres we can pass here just the first of it
	//TODO USE THIS WHEN IS READY
	//uniformGrid.updateGrid(m_sphereSystems[0].getSpheres());

	//get the colliding object. TODO maybe move this to the shpereSystem class
	this->getUniformGridCollidingObjects();
	//test
	//m_sphereSystems[0].printPositions();
}

void SphereSystemSimulator::onClick(int x, int y)
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

void SphereSystemSimulator::onMouse(int x, int y)
{
}

void SphereSystemSimulator::onLeftMouseRelease()
{
	onMouseDown = false;

	//reset mouse positions
	m_mouse = Vec3();
	m_oldtrackmouse = Vec3();
	m_mouseOldLocalCoordinate = Vec3();
	m_mouseLocalCoordinate = Vec3();
}

void SphereSystemSimulator::addSphereSystem(int collisionDetectionMethod, Vec3 color)
{
	SphereSystem sSys = SphereSystem(collisionDetectionMethod, color, m_fRadius, m_fMass, m_fDamping, Vec3(0, -2, 0)); // TODO gravity
	m_sphereSystems.push_back(sSys);
}

void SphereSystemSimulator::addSphere(unsigned char ID, Vec3 pos, Vec3 vel)
{
	// all sphere system should have the same spheres
	for (SphereSystem& s : m_sphereSystems) {
		s.addSphere(ID, pos, vel);
	}
	m_iNumSpheres++;
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
Vec3 SphereSystemSimulator::toLocalCoordinate(Vec3 globalScreenPosition)
{
	return Vec3(globalScreenPosition.x - m_screenWidth / 2, m_screenHeight - globalScreenPosition.y - m_screenHeight / 2, globalScreenPosition.z);
}

void SphereSystemSimulator::changeCameraPosition()
{
	XMFLOAT3 eye(.0f, .0f, -4 * BBOX_SIZE);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

}

// taken from http://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c
void SphereSystemSimulator::getScreenResolution()
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	m_screenWidth = desktop.right;
	m_screenHeight = desktop.bottom;
}

void SphereSystemSimulator::getUniformGridCollidingObjects()
{
	this->m_collidingPairs = uniformGrid.getCollidingPairs();
}
