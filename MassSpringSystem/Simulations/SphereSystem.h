#ifndef SPHERE_SYSTEM_H
#define SPHERE_SYSTEM_H

#include "Simulator.h"

#define BBOX_SIZE 6.0f

// method of collision calculation
#define NAIVEACC 0
#define GRIDACC 1
#define KDACC 2

class SphereSystem {
public:
	struct Sphere
	{
		unsigned char ID; //unique for each object and used for the uniform grid collision detection and thus it should be less then UniformGridCollision.m_maxObjects. It should aways start from 1!
		Vec3 pos;
		Vec3 vel;
	};

	SphereSystem(int collisionDetectionMethod, Vec3 sphereColor, float sphereRadius, float sphereMass, float damping, Vec3 gravity) 
		: m_collDetMethod(collisionDetectionMethod), m_sphereColor(sphereColor), m_fRadius(sphereRadius), m_mass(sphereMass), m_damping(damping), m_gravity(gravity), render(true) {}

	void addSphere(unsigned char ID,Vec3 pos, Vec3 vel);
	void handleCollision(std::vector<Vec3>& forces);
	void collisionResponse(int i, int u, std::vector<Vec3>& forces);
	void draw(DrawingUtilitiesClass * DUC);
	void advanceMidPoint(float timeStep);
	std::vector<Vec3> SphereSystem::ComputeForces();
	void UpdatePositions(float dt);
	void UpdateVelocities(float dt, const std::vector<Vec3>& forces);
	void printPositions();
	bool render;
	std::vector<SphereSystem::Sphere> getSpheres();

private:
	int m_collDetMethod;
	Vec3 m_sphereColor;
	float m_fRadius;
	float m_mass;
	float m_damping;
	Vec3 m_gravity;

	std::vector<Sphere> m_spheres;

};

#endif