#ifndef RIGIDBODYSYSTEMSIMULATOR_h
#define RIGIDBODYSYSTEMSIMULATOR_h
#include "Simulator.h"

#include "util\collisionDetect.h"
#include "RigidBodyCollisionSystem.h"
#include "RigidBody.h"
#include "spring.h"

#define TESTCASEUSEDTORUNTEST 2

// Testcases
#define ONE_STEP_SIMULATION 0
#define SINGLE_BODY_SIMULATION 1
#define DOUBLE_BODY_SIMULATION 2
#define COMPLEX_BODY_SIMULATION 3

//*************Colours*****************
// The rigidbody colour
#define COLOUR_RIGIDBODY Vec3(0.5, 0.5, 0.5)
// The wall colour
#define COLOUR_WALL Vec3(.9, .9, .9)
// The mouse line colour
#define COLOUR_MOUSE_VECTOR Vec3(1,0,0)

// Mouse local Coordinate subtractor. Hardcoded just to look real
#define MOUSE_VECTOR_LENGTH_SUBTRACTOR 210

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
	void onLeftMouseRelease();
	void onMouse(int x, int y);
	void onKeyboardSpaceDown();

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
	void calculateCollision();
	Vec3 toLocalCoordinate(Vec3 globalScreenPosition);
	void applyForceOnEachBody(Vec3 force); //apply force to the center of each rigid body
	

private:
	// Attributes
	RigidBodyCollisionSystem* m_collisionSystem;	// probably not needed

	std::vector<RigidBody> m_rigidBodies; 	//Rigid bodies
	std::vector<RigidBody> m_walls; 		//the rigid bodies of the walls
	
	// UI Attributes
	Vec3 m_externalForce;
	Vec3 m_externalForceLocation;

	Vec3 m_mouse;
	Vec3 m_mouseLocalCoordinate;				//our local coordinate system is with screen center at 0,0,0	
	Vec3 m_mouseOldLocalCoordinate;				//our old local coordinate system is with screen center at 0,0,0	
	Vec3 m_oldtrackmouse;

	bool onMouseDown;

	// Wall coordinates and camera eye location
	float wallOffset;

	void buildTower(Vec3 position, Vec3 size, Vec3 boxSize, float boxMass);
	void initWalls();				//usses the wallOffset variable
	void changeCameraPosition();	//usses the wallOffset variable
	void resetWalls();

	};
#endif