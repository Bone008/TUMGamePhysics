#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h

#include "Simulator.h"
#include "SphereSystem.h"
#include "UniformGridCollision.h"

// testcases
#define TEST_NAIVE     0
#define TEST_ACCEL	   1
#define TEST_PERF_COMP 2

//defines the subtractor of the used resolution(width, height and depht) for our uniform gird array 
#define RESOLUTION_CORRECTOR 64

// The mouse line colour
#define COLOUR_MOUSE_VECTOR Vec3(1,0,0)

// Mouse local Coordinate subtractor. Hardcoded just to look real
#define MOUSE_VECTOR_LENGTH_SUBTRACTOR 210

class SphereSystemSimulator:public Simulator{
public:
	// Construtors
	SphereSystemSimulator();
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	void onLeftMouseRelease();

	void addSphereSystem(int collisionDetectionMethod, Vec3 color);
	void addSphere(unsigned char ID,Vec3 pos, Vec3 vel);

	Vec3 toLocalCoordinate(Vec3 globalScreenPosition);
protected:
	// Attributes
	Vec3 externalForce;
	float m_fMass;
	float m_fRadius;
	float m_fForceScaling;
	float m_fDamping;
	int   m_iNumSpheres;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	
	std::vector<SphereSystem> m_sphereSystems;

	void changeCameraPosition();    //usses the BBOX_SIZE variable
	void getScreenResolution();		//take if before the directX initialization

	Vec3 m_mouse;
	Vec3 m_mouseLocalCoordinate;				//our local coordinate system is with screen center at 0,0,0	
	Vec3 m_mouseOldLocalCoordinate;				//our old local coordinate system is with screen center at 0,0,0	
	Vec3 m_oldtrackmouse;

	bool onMouseDown;

	//SphereSystem * m_pSphereSystem; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	// SphereSystem * m_pSphereSystemGrid; 

	UniformGridCollision uniformGrid;
	//object holder for the colliding pairs from the uniform grid class
	std::vector<UniformGridCollision::CollidingSpheresPair> m_collidingPairs;
	void getUniformGridCollidingObjects();	//getter for the colliding object from the uniform grid class
};

#endif