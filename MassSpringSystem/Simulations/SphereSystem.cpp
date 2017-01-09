#include "SphereSystem.h"

void SphereSystem::addSphere(unsigned char ID, Vec3 pos, Vec3 vel)
{
	Sphere s;
	s.ID = ID;
	s.pos = pos;
	s.vel = vel;
	m_spheres.push_back(s);
}

void SphereSystem::handleCollision(std::vector<Vec3>& forces)
{
	for (int i = 0; i < m_spheres.size(); i++)
	{
		Vec3 pos = m_spheres[i].pos;
		Vec3 vel = m_spheres[i].vel;

		for (int f = 0; f < 6; f++)
		{
			float sign = (f % 2 == 0) ? -1.0f : 1.0f;
			if (sign * pos.value[f / 2] < -(BBOX_SIZE - m_fRadius))
			{
				pos.value[f / 2] = sign * -(BBOX_SIZE-m_fRadius);
				vel.value[f / 2] = 0;
			}
		}

		m_spheres[i].pos = pos;
		m_spheres[i].vel = vel;
	}
	
	switch (m_collDetMethod)
	{
	case NAIVEACC:
		// brute force
		for (int i = 0; i < m_spheres.size(); i++)
		{
			for (int u = i + 1; u < m_spheres.size(); u++) {
				const double sqDist = m_spheres[i].pos.squaredDistanceTo(m_spheres[u].pos);
				const double diameter = 2 * m_fRadius;

				// |d2-d1| < 2r --> collision
				if (sqDist < diameter * diameter) {
					collisionResponse(i, u, forces);
				}
			}
		}
		break;

	case GRIDACC:
		// TODO
		break;

	case KDACC:
		// TODO
		break;
	}
}

// TODO wrong!
void SphereSystem::collisionResponse(int i, int u, std::vector<Vec3>& forces)
{
	const double sqDist = m_spheres[i].pos.squaredDistanceTo(m_spheres[u].pos);
	const double diameter = 2 * m_fRadius;

	const double lambda = 20.0f; // TODO
	
	const double f = lambda * (1 - (sqrt(sqDist) / diameter));

	const Vec3 n = m_spheres[i].pos - m_spheres[u].pos;

	forces[i] += f * n;
	forces[u] -= f * n;
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

	// Repulsion forces
	handleCollision(forces);

	// Damping forces	
	for (size_t i = 0; i < m_spheres.size(); i++)
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

void SphereSystem::UpdateVelocities(float dt, Vec3 force)
{
	for (size_t i = 0; i<m_spheres.size(); i++)
	{
		Vec3 vel = m_spheres[i].vel;
		float m = m_mass;

		vel += force * (dt / m);

		m_spheres[i].vel = vel;
	}
}

void SphereSystem::printPositions()
{
	for (Sphere s : m_spheres)
		cout << s.pos << '\t';
	cout << endl;
}

std::vector<SphereSystem::Sphere> SphereSystem::getSpheres()
{
	return m_spheres;
}


