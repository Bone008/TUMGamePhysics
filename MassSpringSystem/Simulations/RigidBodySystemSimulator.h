#ifndef RIGIDBODYSYSTEMSIMULATOR_h
#define RIGIDBODYSYSTEMSIMULATOR_h
#include "Simulator.h"

#include "RigidBodySystem.h"
#include "RigidBody.h"

#define TESTCASEUSEDTORUNTEST 2

// Testcases
#define ONE_STEP_SIMULATION 0
#define SINGLE_BODY_SIMULATION 1
#define DOUBLE_BODY_SIMULATION 2
#define COMPLEX_BODY_SIMULATION 3

class RigidBodySystemSimulator:public Simulator{
public:
	// Construtors
	RigidBodySystemSimulator();
	
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

	// ExtraFunctions
	int getNumberOfRigidBodies();
	Vec3 getPositionOfRigidBody(int i);
	Vec3 getLinearVelocityOfRigidBody(int i);
	Vec3 getAngularVelocityOfRigidBody(int i);
	void applyForceOnBody(int i, Vec3 loc, Vec3 force);
	void addRigidBody(Vec3 position, Vec3 size, int mass);
	void addRigidBody(Vec3 position, Quat orientation, Vec3 size, int mass);
	void setOrientationOf(int i,Quat orientation);
	void setVelocityOf(int i, Vec3 velocity);

private:
	// Attributes
	RigidBodySystem* m_pRigidBodySystem; // probably not needed

	std::vector<RigidBody> m_rigidBodies; 	//Rigid bodies
	std::vector<RigidBody> m_walls; 		//the rigid bodies of the walls
	

	// UI Attributes
	Vec3 m_externalForce;
	Vec3 m_externalForceLocation;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	void buildTower(Vec3 position, Vec3 size, Vec3 boxSize, float boxMass);
	void initWalls();
	};
#endif