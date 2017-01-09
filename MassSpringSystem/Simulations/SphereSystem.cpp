#include "SphereSystem.h"

void SphereSystem::addSphere(Vec3 pos, Vec3 vel)
{
	Sphere s;
	s.pos = pos;
	s.vel = vel;
	m_spheres.push_back(s);
}


void SphereSystem::advanceLeapfrog(float dt, DrawingUtilitiesClass* DUC, boolean onMouseDown, Vec3 mouseForce)
{

	// disabled midpoint integration because it was messing with velocities changed
	// by wall collisions (within ComputeForces()). couldn't think of a simple way to refactor that,
	// so I switched to leapfrog integration, which is also 2nd order (because of magic), but has no resets

	/*
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
	*/


	// use leap-frog because it's more simple
	ComputeForces(DUC);
	UpdateVelocities(dt);
	UpdatePositions(dt);
	
	// update velocites from mouse interaction
	if (onMouseDown) {
		for (size_t i = 0; i < m_spheres.size(); i++)
		{
			Vec3 vel = m_spheres[i].vel;
			float m = m_mass;

			vel += mouseForce * (dt / m);

			m_spheres[i].vel = vel;
		}
	}
}



void SphereSystem::handleCollisions()
{
	for (Sphere& sphere : m_spheres)
	{
		Vec3 pos = sphere.pos;
		Vec3 vel = sphere.vel;

		for (int f = 0; f < 6; f++)
		{
			float sign = (f % 2 == 0) ? -1.0f : 1.0f;
			if (sign * pos.value[f / 2] < -(BBOX_SIZE - m_fRadius))
			{
				pos.value[f / 2] = sign * -(BBOX_SIZE-m_fRadius - 0.001f);

				if (vel.value[f / 2] * sign < 0)
					vel.value[f / 2] *= -0.6; // bounce off walls, but lose some energy along the way
			}
		}

		sphere.pos = pos;
		sphere.vel = vel;
	}
	

	switch (m_collDetMethod)
	{

	case NAIVEACC:
	{
		// brute force
		for (unsigned int i = 0; i < m_spheres.size(); i++)
		{
			for (unsigned int u = i + 1; u < m_spheres.size(); u++) {
				const double sqDist = m_spheres[i].pos.squaredDistanceTo(m_spheres[u].pos);
				const double diameter = 2 * m_fRadius;

				// |d2-d1| < 2r --> collision
				if (sqDist < diameter * diameter) {
					//cout << "[naiv] response to pair " << &m_spheres[i] << ", " << &m_spheres[u] << endl;
					collisionResponse(m_spheres[i], m_spheres[u]);
				}
			}
		}
		break;
	}

	case GRIDACC:
	{
		m_uniformGrid.updateGrid(m_spheres);
		const std::unordered_set<std::pair<Sphere*, Sphere*>>& possiblyCollidingPairs = m_uniformGrid.computeCollisionPairs();

		//cout << "pairs: " << possiblyCollidingPairs.size() << endl;

		for (auto& pair : possiblyCollidingPairs)
		{
			const double sqDist = pair.first->pos.squaredDistanceTo(pair.second->pos);
			const double diameter = 2 * m_fRadius;

			// |d2-d1| < 2r --> collision
			if (sqDist < diameter * diameter) {
				//cout << "[grid] response to pair " << pair.first << ", " << pair.second << endl;
				collisionResponse(*pair.first, *pair.second);
			}
		}
		break;
	}

	case KDACC:
		// TODO
		break;
	}
}


void SphereSystem::collisionResponse(Sphere& sphere1, Sphere& sphere2)
{
	const double sqDist = sphere1.pos.squaredDistanceTo(sphere2.pos);
	const double diameter = 2 * m_fRadius;

	const double lambda = 250.0f; // TODO tweak
	
	const double f = lambda * m_kernel(sqrt(sqDist) / diameter);

	//std::cout << "collision! " << i << "," << u << " - " << (1 - (sqrt(sqDist) / diameter)) << std::endl;

	Vec3 n = sphere1.pos - sphere2.pos;
	normalize(n); // unit length!!

	sphere1.computedForce += f * n;
	sphere2.computedForce -= f * n;
}


void SphereSystem::ComputeForces(DrawingUtilitiesClass* DUC)
{
	// reset forces
	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce = Vec3();
	}

	// Repulsion forces
	handleCollisions();

	// Gravity & Damping forces
	Vec3 gravityForce = m_gravity * m_mass;
	if (m_camRotDependentGravity)
	{
		// direction of gravity will be the bottom of the screen
		Mat4 viewMatrix(DUC->g_camera.GetViewMatrix());
		gravityForce = viewMatrix * gravityForce;
	}
		
	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce += gravityForce;
		sphere.computedForce += sphere.vel * -m_damping;
	}
}

void SphereSystem::UpdatePositions(float dt)
{
	for (Sphere& sphere : m_spheres)
	{
		sphere.pos += sphere.vel * dt;
	}
}

void SphereSystem::UpdateVelocities(float dt)
{
	for (Sphere& sphere : m_spheres)
	{
		sphere.vel += sphere.computedForce / m_mass * dt;
	}
}



void SphereSystem::draw(DrawingUtilitiesClass * DUC, boolean onMouseDown, Vec3 mouseLocalCoordinate)
{
	if (render) {
		// set up lighting, all spheres of one system have the same color
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, m_sphereColor);

		// draw every sphere
		for (const Sphere& s : m_spheres) {
			DUC->drawSphere(s.pos, Vec3(m_fRadius, m_fRadius, m_fRadius));

			//draw the mouse spring
			if (onMouseDown) {
				DUC->beginLine();
				DUC->drawLine(s.pos, COLOUR_MOUSE_VECTOR, (mouseLocalCoordinate / MOUSE_VECTOR_LENGTH_SUBTRACTOR), COLOUR_MOUSE_VECTOR);
				DUC->endLine();
			}
		}
	}
}
