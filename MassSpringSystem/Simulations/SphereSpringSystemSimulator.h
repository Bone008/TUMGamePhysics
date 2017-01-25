#ifndef SPHERESPRINGSYSTEMSIMULATOR_H
#define SPHERESPRINGSYSTEMSIMULATOR_H

#include "Simulator.h"
#include "SphereSpringSystem.h"

// testcases
#define TEST_FIRST 0
#define TEST_SECOND	1

// for shooting things
#define SHOOT_SPHERE_SPRING_DIMENSION Vec3(0.5, 0, 0)
#define SHOOT_SPHERE_SPRING_VELOCITY Vec3(0, 0, 1000)

// camera
#define CAMERA_POSITION_Z (-3.5f * BBOX_HALF_SIZE)

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
	void onKeyboardSpaceDown();

	Vec3 toLocalCoordinate(Vec3 globalScreenPosition);

private:
	std::unique_ptr<SphereSpringSystem> m_SphereSpringSystem;

	float m_stiffness;
	float m_breakThreshold;
	float m_damping;
	Vec3 m_gravity;
	bool m_camRotDependentGravity;

	int m_gridCells;
	int m_gridCellCapacity;

	void buildTower(Vec3 pos, Vec3 size);

	void changeCameraPosition();    //usses the BBOX_HALF_SIZE variable

	Vec3 m_mouse;
	Vec3 m_mouseLocalCoordinate;		//our local coordinate system is with screen center at 0,0,0    
	Vec3 m_mouseOldLocalCoordinate;     //our old local coordinate system is with screen center at 0,0,0    
	Vec3 m_oldtrackmouse;
	Vec3 m_mouseShootingPosition;

	bool onMouseDown;
	bool initComplete;
};

#endif