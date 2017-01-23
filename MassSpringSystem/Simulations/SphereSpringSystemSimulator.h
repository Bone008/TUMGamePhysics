#ifndef SPHERESPRINGSYSTEMSIMULATOR_H
#define SPHERESPRINGSYSTEMSIMULATOR_H

#include "Simulator.h"
#include "SphereSpringSystem.h"

// testcases
#define TEST_FIRST 0
#define TEST_SECOND	1

class SphereSpringSystemSimulator : public Simulator {
public:
	// constructor
	SphereSpringSystemSimulator();

	// simulator functions
	void reset();

	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void notifyCaseChanged(int testCase);

	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);

	void onClick(int x, int y);
	void onLeftMouseRelease();
	void onMouse(int x, int y);

	Vec3 toLocalCoordinate(Vec3 globalScreenPosition);

private:
	SphereSpringSystem* m_SphereSpringSystem;

	float m_stiffness;
	float m_damping;
	float m_mass;
	Vec3 m_gravity;

	void buildTower(Vec3 pos, Vec3 size);

	void changeCameraPosition();    //usses the BBOX_HALF_SIZE variable

	Vec3 m_mouse;
	Vec3 m_mouseLocalCoordinate;		//our local coordinate system is with screen center at 0,0,0    
	Vec3 m_mouseOldLocalCoordinate;     //our old local coordinate system is with screen center at 0,0,0    
	Vec3 m_oldtrackmouse;

	bool onMouseDown;
	bool initComplete;
};

#endif