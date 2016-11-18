#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h

#include "Simulator.h"
#include "point.h"
#include "spring.h"

// Do Not Change
#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2
// Do Not Change

// Testcases
#define ONE_STEP_TEST 0
#define TWO_POINT_SETUP 1
#define COMPLEX_SETUP 2

// Bounding Box
#define BBOX_CENTER Vec3()
#define BBOX_SIZE 0.5f

#define MAX_MOUSE_DISTANCE 50
#define MAGIC_NUMBER_MOUSE_SUBTRACTION 1000

class MassSpringSystemSimulator:public Simulator{
public:
	//Construtors
	MassSpringSystemSimulator();
	
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

	// Specific Functions
	void setIntegrator(int integrator);
	void setMass(float mass);
	void setStiffness(float stiffness);
	void setDampingFactor(float damping);
	int addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed);
	void addSpring(int masspoint1, int masspoint2, float initialLength);
	int getNumberOfMassPoints();
	int getNumberOfSprings();
	Vec3 getPositionOfMassPoint(int index);
	Vec3 getVelocityOfMassPoint(int index);
	void applyExternalForce(Vec3 force);

	//Point2D functions. TODO Make class just for it
	Point2D setBoth(int x, int y);
	bool isSet(Point2D p);
	Vec3 getMouseDirection(Point2D mouse, Point2D oldMouse);
	int getDistance(Point2D a, Point2D b);
	

private:
	// Masspoints and Springs
	std::vector<point> m_massPoints;
	std::vector<spring> m_springs;

	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;
	int m_iIntegrator;

	//For leap-frog
	bool firstTime;
	
	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	bool m_collision;
	
	// Force and integrating functions
	void clearForces();
	void computeElasticForces();
	void integrate(float timeStep);
	void integrateEuler(float timeStep);
	void integrateMidpoint(float timeStep);
	void integrateLeapfrog(float timeStep);
	void validatePointPosition(point& p);

	void changePosition(Vec3 externalForce);
};
#endif