#ifndef SPHERE_SYSTEM_H
#define SPHERE_SYSTEM_H

#include "Simulator.h"
#include "Sphere.h"
#include "UniformGrid.h"

#define BBOX_SIZE 6.0

// method of collision calculation
#define NAIVEACC 0
#define GRIDACC 1
#define KDACC 2

// The mouse line colour
#define COLOUR_MOUSE_VECTOR Vec3(1,0,0)

// Mouse local Coordinate subtractor. Hardcoded just to look real
#define MOUSE_VECTOR_LENGTH_SUBTRACTOR 210

class SphereSystem {
public:
	SphereSystem(int collisionDetectionMethod, Vec3 sphereColor, float sphereRadius, float sphereMass, float damping, Vec3 gravity, int gridCells, int cellCapacity, std::function<float(float)> kernel, boolean camRotDependentGravity)
		: m_collDetMethod(collisionDetectionMethod), m_sphereColor(sphereColor), m_fRadius(sphereRadius),
		  m_mass(sphereMass), m_damping(damping), m_gravity(gravity), render(true),
		  m_uniformGrid(BBOX_SIZE, gridCells, cellCapacity, sphereRadius),
		  m_kernel(kernel), m_camRotDependentGravity(camRotDependentGravity){
	}

	void addSphere(Vec3 pos, Vec3 vel);
	void advanceLeapfrog(float timeStep, DrawingUtilitiesClass* DUC, boolean onMouseDown, Vec3 mouseForce);
	void handleCollisions();
	void collisionResponse(Sphere& sphere1, Sphere& sphere2);
	void ComputeForces(DrawingUtilitiesClass* DUC);
	void UpdateVelocities(float dt);
	void UpdatePositions(float dt);

	void draw(DrawingUtilitiesClass * DUC, boolean onMouseDown, Vec3 mouseLocalCoordinate);
	bool render;

	int m_collDetMethod;
private:
	Vec3 m_sphereColor;
	float m_fRadius;
	float m_mass;
	float m_damping;
	Vec3 m_gravity;

	std::vector<Sphere> m_spheres;
	UniformGrid m_uniformGrid;

	std::function<float(float)> m_kernel;

	boolean m_camRotDependentGravity;

};

#endif