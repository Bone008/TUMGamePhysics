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
		Vec3 pos;
		Vec3 vel;
		float mass;
	};

	SphereSystem(int collisionDetectionMethod, Vec3 sphereColor, float sphereRadius) 
		: m_collDetMethod(collisionDetectionMethod), m_sphereColor(sphereColor), m_fRadius(sphereRadius), render(true) {}

	void addSphere(Vec3 pos, Vec3 vel,float mass);
	void handleCollision();
	void collisionResponse(Sphere& a, Sphere& b);
	void collisionResponseWall(Sphere& a, double dist, Vec3 direction);
	void draw(DrawingUtilitiesClass * DUC);
	void simulateTimestep(float timeStep);

	bool render;

private:
	int m_collDetMethod;
	Vec3 m_sphereColor;
	float m_fRadius;

	std::vector<Sphere> m_spheres;

};

#endif