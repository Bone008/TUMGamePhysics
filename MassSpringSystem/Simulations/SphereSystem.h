#ifndef SPHERE_SYSTEM_H
#define SPHERE_SYSTEM_H

#include "Simulator.h"

class SphereSystem {
public:
	struct Sphere
	{
		Vec3 pos;
		Vec3 vel;
	};

	SphereSystem(Vec3 sphereColor, float sphereRadius) : m_sphereColor(sphereColor), m_fRadius(sphereRadius), render(true) {}

	void addSphere(Vec3 pos, Vec3 vel);
	void draw(DrawingUtilitiesClass * DUC);
	void simulateTimestep(float timeStep);

	bool render;

private:
	Vec3 m_sphereColor;
	float m_fRadius;

	std::vector<Sphere> m_spheres;

};

#endif