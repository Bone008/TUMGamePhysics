#ifndef SPHERESPRINGSYSTEM_H
#define SPHERESPRINGSYSTEM_H

#include "Sphere.h"
#include "SphereSpring.h"
#include "UniformGrid.h"

#define BBOX_HALF_SIZE 6.0

#define SPHERE_RADIUS 0.1
#define SPHERE_COLOR Vec3(0.8, 0.2, 0.2) // red
#define SPRING_COLOR Vec3(0.2, 0.5, 0.8) // blue

class SphereSpringSystem {
public:
	SphereSpringSystem(float springStiffness, float springDamping, float sphereMass, int gridCells, int cellCapacity) : 
		m_stiffness(springStiffness), m_damping(springDamping), m_mass(sphereMass),
		m_uniformGrid(BBOX_HALF_SIZE, gridCells, cellCapacity, SPHERE_RADIUS) {}

	int addSphere(Vec3 pos, Vec3 vel);
	void addSpring(int sphInd1, int sphInd2, float initialLength);
	void addSpring(int sphInd1, int sphInd2);
	void draw(DrawingUtilitiesClass* DUC);

	void advanceLeapFrog(float timeStep);

private:
	std::vector<Sphere> m_spheres;
	std::vector<SphereSpring> m_springs;

	float m_stiffness;
	float m_damping;
	float m_mass;

	UniformGrid m_uniformGrid;

	void computeForces();
	void updatePositions(float timeStep);
	void updateVelocities(float timeStep);
	void handleCollisions();
	void collisionResponse(Sphere& sphere1, Sphere& sphere2);
};

#endif