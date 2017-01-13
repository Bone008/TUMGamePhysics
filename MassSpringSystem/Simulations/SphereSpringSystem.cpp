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

void SphereSpringSystem::advanceLeapFrog(float timeStep)
{
	computeForces();
	updateVelocities(timeStep);
	updatePositions(timeStep);
}

void SphereSpringSystem::computeForces()
{
	// reset forces
	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce = Vec3();
	}

	// TODO repulsion force
	// TODO gravity

	// spring forces
	for (SphereSpring& spring : m_springs)
	{
		// Spring forces
		int s1 = spring.sphere1;
		int s2 = spring.sphere2;
		Vec3 pos1 = m_spheres[s1].pos;
		Vec3 pos2 = m_spheres[s2].pos;

		Vec3 d = pos1 - pos2;
		float l = norm(d);
		float L = spring.initialLength;
		float k = m_stiffness;

		Vec3 f = d * (-k * (l - L) / l);

		m_spheres[s1].computedForce += f;
		m_spheres[s2].computedForce -= f;
	}

	// damping forces
	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce += sphere.vel * -m_damping;
	}
}

void SphereSpringSystem::updatePositions(float timeStep)
{
	for (Sphere& sphere : m_spheres)
	{
		Vec3 pos = sphere.pos;
		Vec3 vel = sphere.vel;

		pos += vel * timeStep;
		sphere.pos = pos;
	}
}

void SphereSpringSystem::updateVelocities(float timeStep)
{
	for (Sphere& sphere : m_spheres)
	{
		Vec3 vel = sphere.vel;
		float m = m_mass;

		vel += sphere.computedForce * (timeStep / m);

		sphere.vel = vel;
	}
}
