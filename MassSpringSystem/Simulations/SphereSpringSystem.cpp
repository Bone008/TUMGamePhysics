#include "SphereSpringSystem.h"

int SphereSpringSystem::addSphere(Vec3 pos, Vec3 vel)
{
	Sphere s;
	s.pos = pos;
	s.vel = vel;
	s.computedForce = Vec3();

	m_spheres.push_back(s);

	return (int) m_spheres.size() - 1;
}

void SphereSpringSystem::addSpring(int sphInd1, int sphInd2, float initialLength)
{
	SphereSpring s;
	s.sphere1 = sphInd1;
	s.sphere2 = sphInd2;
	s.initialLength = initialLength;

	m_springs.push_back(s);
}

void SphereSpringSystem::addSpring(int sphInd1, int sphInd2)
{
	Vec3 distance = m_spheres[sphInd1].pos - m_spheres[sphInd2].pos;
	addSpring(sphInd1, sphInd2, norm(distance));
}

void SphereSpringSystem::draw(DrawingUtilitiesClass* DUC)
{
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1), 100, 0.6 * SPHERE_COLOR);

	// draw spheres
	for (Sphere& s : m_spheres)
	{
		DUC->drawSphere(s.pos, Vec3(SPHERE_RADIUS));
	}

	// draw springs
	DUC->beginLine();
	for (SphereSpring& s : m_springs)
	{
		Sphere s1 = m_spheres[s.sphere1];
		Sphere s2 = m_spheres[s.sphere2];
		DUC->drawLine(s1.pos, SPRING_COLOR, s2.pos, SPRING_COLOR);
	}
	DUC->endLine();
}
