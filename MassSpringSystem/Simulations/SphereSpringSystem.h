#ifndef SPHERESPRINGSYSTEM_H
#define SPHERESPRINGSYSTEM_H

#include "Sphere.h"
#include "SphereSpring.h"
#include "UniformGrid.h"

#define BBOX_HALF_SIZE 6.0

#define SPHERE_COLOR Vec3(0.8, 0.2, 0.2) // red
#define SPRING_COLOR Vec3(0.2, 0.5, 0.8) // blue

class SphereSpringSystem {
public:
	SphereSpringSystem(float springStiffness, float springDamping, float sphereMass, Vec3 gravity, bool camRotDependentGravity, int gridCells, int cellCapacity) :
		m_stiffness(springStiffness), m_damping(springDamping), m_mass(sphereMass), m_gravity(gravity), m_camRotDependentGravity(camRotDependentGravity),
		m_uniformGrid(BBOX_HALF_SIZE, gridCells, cellCapacity) {}

	int addSphere(Vec3 pos, Vec3 vel, float radius);
	void addSpring(int sphInd1, int sphInd2, float initialLength);
	void addSpring(int sphInd1, int sphInd2);
	void draw(DrawingUtilitiesClass* DUC);

	void advanceLeapFrog(float timeStep, DrawingUtilitiesClass* duc, bool onMouseDown, Vec3 mouseForce);

private:
	std::vector<Sphere> m_spheres;
	std::vector<SphereSpring> m_springs;

	float m_stiffness;
	float m_damping;
	float m_mass;
	Vec3 m_gravity;
	bool m_camRotDependentGravity;

	UniformGrid m_uniformGrid;

	void computeForces(DrawingUtilitiesClass* DUC);
	void updatePosAndVel(float timeStep);
	void handleCollisions();
	void collisionResponse(Sphere& sphere1, Sphere& sphere2);
};

#endif