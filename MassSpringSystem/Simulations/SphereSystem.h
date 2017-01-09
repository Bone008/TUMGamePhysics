#ifndef SPHERE_SYSTEM_H
#define SPHERE_SYSTEM_H

#include "Simulator.h"
#include "Sphere.h"
#include "UniformGrid.h"

#define BBOX_SIZE 6.0f

// method of collision calculation
#define NAIVEACC 0
#define GRIDACC 1
#define KDACC 2

class SphereSystem {
public:
	SphereSystem(int collisionDetectionMethod, Vec3 sphereColor, float sphereRadius, float sphereMass, float damping, Vec3 gravity) 
		: m_collDetMethod(collisionDetectionMethod), m_sphereColor(sphereColor), m_fRadius(sphereRadius), m_mass(sphereMass), m_damping(damping), m_gravity(gravity), render(true) {}

	void addSphere(Vec3 pos, Vec3 vel);
	void advanceMidPoint(float timeStep);
	void handleCollisions();
	void collisionResponse(Sphere& sphere1, Sphere& sphere2);
	void ComputeForces();
	void UpdateVelocities(float dt);
	void UpdatePositions(float dt);

	void draw(DrawingUtilitiesClass * DUC);
	bool render;

private:
	int m_collDetMethod;
	Vec3 m_sphereColor;
	float m_fRadius;
	float m_mass;
	float m_damping;
	Vec3 m_gravity;

	std::vector<Sphere> m_spheres;
	UniformGrid m_uniformGrid;

};

#endif