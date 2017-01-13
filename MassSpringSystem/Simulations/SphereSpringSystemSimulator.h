#ifndef SPHERESPRINGSYSTEMSIMULATOR_H
#define SPHERESPRINGSYSTEMSIMULATOR_H

#include "Simulator.h"
#include "SphereSpringSystem.h"

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
	
private:
	SphereSpringSystem* m_SphereSpringSystem;

	float m_stiffness;
	float m_damping;
	float m_mass;
	Vec3 m_gravity;
};

#endif