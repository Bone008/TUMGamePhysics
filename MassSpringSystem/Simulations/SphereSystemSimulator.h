#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h

#include "Simulator.h"
#include "SphereSystem.h"

// testcases
#define TEST_SIMPLE     0
#define TEST_COMPLEX	   1
#define TEST_PERF_COMP 2

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
	void addSphere(Vec3 pos, Vec3 vel);

protected:
	// Attributes
	Vec3 externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	// UI stuff
	float m_fMass;
	float m_fRadius;
	float m_fDamping;
	int   m_iNumSpheres;
	Vec3 m_gravity;

	bool m_useNaive, m_useGrid;
	int m_iGridCells;
	int m_iGridCapacity;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	std::vector<SphereSystem> m_sphereSystems;

	boolean m_camRotDependentGravity;

	void changeCameraPosition();    //usses the BBOX_SIZE variable

};

#endif