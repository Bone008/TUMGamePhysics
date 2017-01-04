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

// TODO wrong!
void SphereSystem::collisionResponse(Sphere & a, Sphere & b)
{
	const double sqDist = a.pos.squaredDistanceTo(b.pos);

	const double lambda = 1.0f; // TODO
	const double f = lambda * (1 - (sqrt(sqDist) / (2 * m_fRadius)));

	const Vec3 n = a.pos - b.pos;
	a.vel += f * n;
	b.vel -= f * n;
}

// TODO wrong? 
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

void SphereSystem::advanceMidPoint(float dt)
{
	std::vector<Sphere> ori_points = m_spheres;// save x_old, v_old

	std::vector<Vec3> forces = ComputeForces();// force = a( x_old, v_old), force_old
	UpdatePositions(dt / 2.0f); // x = x_tmp, using v_old
	UpdateVelocities(dt / 2.0f, forces); // v = v_tmp, using force_old

	forces = ComputeForces();// force = a ( x_tmp, v_tmp )

	for (size_t i = 0; i < m_spheres.size(); i++)//restore x_old
	{
		m_spheres[i].pos = ori_points[i].pos;
	}
	UpdatePositions(dt);// x = x ( vtmp )
	for (size_t i = 0; i < m_spheres.size(); i++)//restore v_old
	{
		m_spheres[i].vel = ori_points[i].vel;
	}
	UpdateVelocities(dt, forces);// v = v( a (xtmp, vtmp) )
}

std::vector<Vec3> SphereSystem::ComputeForces()
{
	// Gravity forces

	std::vector<Vec3> forces(m_spheres.size(), m_gravity * m_mass);

	for (size_t i = 0; i<m_spheres.size(); i++)
	{
		// repulsion force here 
		/*
		int p1 = m_springs[i].point1;
		int p2 = m_springs[i].point2;
		Vec3 pos1 = m_spheres[p1].pos;
		Vec3 pos2 = m_spheres[p2].pos;

		Vec3 d = pos1 - pos2;
		float l = norm(d);
		float L = m_springs[i].initialLength;
		float k = m_stiffness;

		Vec3 f = d * (-k * (l - L) / l);

		forces[p1] += f;
		forces[p2] -= f;
		*/
	}

	// Damping forces
	for (size_t i = 0; i<m_spheres.size(); i++)
	{
		Vec3 vel = m_spheres[i].vel;

		forces[i] += vel * -m_damping;
	}
	return forces;
}

void SphereSystem::UpdatePositions(float dt)
{
	for (size_t i = 0; i<m_spheres.size(); i++)
	{
		Vec3 pos = m_spheres[i].pos;
		Vec3 vel = m_spheres[i].vel;

		pos += vel * dt;
		m_spheres[i].pos = pos;
	}
}

void SphereSystem::UpdateVelocities(float dt, const std::vector<Vec3>& forces)
{
	for (size_t i = 0; i<m_spheres.size(); i++)
	{
		Vec3 vel = m_spheres[i].vel;
		float m = m_mass;

		vel += forces[i] * (dt / m);

		m_spheres[i].vel = vel;
	}
}