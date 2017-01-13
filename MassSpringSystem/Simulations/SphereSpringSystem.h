#ifndef SPHERESPRINGSYSTEM_H
#define SPHERESPRINGSYSTEM_H

#include "Sphere.h"
#include "SphereSpring.h"

#define SPHERE_RADIUS 0.1
#define SPHERE_COLOR Vec3(0.8, 0.2, 0.2) // red
#define SPRING_COLOR Vec3(0.2, 0.5, 0.8) // blue

class SphereSpringSystem {
public:
	SphereSpringSystem() {}

	int addSphere(Vec3 pos, Vec3 vel);
	void addSpring(int sphInd1, int sphInd2, float initialLength);
	void addSpring(int sphInd1, int sphInd2);
	void draw(DrawingUtilitiesClass* DUC);

private:
	std::vector<Sphere> m_spheres;
	std::vector<SphereSpring> m_springs;

};

#endif