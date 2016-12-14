#include "SphereSystem.h"

void SphereSystem::addSphere(Vec3 pos, Vec3 vel)
{
	Sphere s;
	s.pos = pos;
	s.vel = vel;
	m_spheres.push_back(s);
}

void SphereSystem::handleCollision()
{
	// TODO apply collision detection methods above to walls, too?
	{
		for (Sphere& s : m_spheres) {
			// collision with bbox
			const Vec3 distMin = s.pos - m_fRadius + BBOX_SIZE;
			if (distMin.x < 0)
				collisionResponseWall(s, distMin.x, Vec3(1, 0, 0));
			if (distMin.y < 0)
				collisionResponseWall(s, distMin.y, Vec3(0, 1, 0));
			if (distMin.z < 0)
				collisionResponseWall(s, distMin.z, Vec3(0, 0, 1));

			const Vec3 distMax = -s.pos - m_fRadius + BBOX_SIZE;
			if (distMax.x < 0)
				collisionResponseWall(s, distMax.x, Vec3(-1, 0, 0));
			if (distMax.y < 0)
				collisionResponseWall(s, distMax.y, Vec3(0, -1, 0));
			if (distMax.z < 0)
				collisionResponseWall(s, distMax.z, Vec3(0, 0, -1));
		}
	}

	switch (m_collDetMethod)
	{
	case NAIVEACC:
		// brute force testing
		for (int i = 0; i < m_spheres.size(); i++) {			
			// collision with other spheres
			for (int u = i + 1; u < m_spheres.size(); u++) {
				const double sqDist = m_spheres[i].pos.squaredDistanceTo(m_spheres[u].pos);
				const double diameter = 2 * m_fRadius;
				
				// |d2-d1| < 2r --> collision
				if (sqDist < diameter * diameter) {
					collisionResponse(m_spheres[i], m_spheres[u]);
				}
			}
		}
		break;

	case GRIDACC:
		break;

	case KDACC:
		break;
	}
}

void SphereSystem::collisionResponse(Sphere & a, Sphere & b)
{
	const double sqDist = a.pos.squaredDistanceTo(b.pos);

	const double lambda = 1.0f; // TODO
	const double f = lambda * (1 - (sqrt(sqDist) / (2 * m_fRadius)));

	const Vec3 n = a.pos - b.pos;
	a.vel += f * n; // TODO / mass
	b.vel -= f * n; // TODO / mass
}

void SphereSystem::collisionResponseWall(Sphere & a, double dist, Vec3 direction)
{
	const double lambda = 1.0f; // TODO

	const double f = lambda * (1 - (dist / (2 * m_fRadius)));
	a.vel -= direction * dist * f; // TODO / mass
}

void SphereSystem::draw(DrawingUtilitiesClass * DUC)
{
	if (render) {
		// set up lighting, all spheres of one system have the same color
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, m_sphereColor);

		// draw every sphere
		for (const Sphere& s : m_spheres) {
			DUC->drawSphere(s.pos, Vec3(m_fRadius, m_fRadius, m_fRadius));
		}
	}
}

void SphereSystem::simulateTimestep(float timeStep)
{
	for (Sphere& s : m_spheres) {
		// integrate position
		s.pos += timeStep * s.vel;
	}
}
