#include "SphereSpringSystem.h"

int SphereSpringSystem::addSphere(Vec3 pos, Vec3 vel, float radius)
{
	Sphere s;
	s.pos = pos;
	s.vel = vel;
	s.computedForce = Vec3();
	s.radius = radius;

	m_spheres.push_back(s);

	return (int)m_spheres.size() - 1;
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
	const Vec3 distance = m_spheres[sphInd1].pos - m_spheres[sphInd2].pos;
	addSpring(sphInd1, sphInd2, norm(distance));
}

void SphereSpringSystem::draw(DrawingUtilitiesClass* DUC)
{
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1), 100, 0.6 * SPHERE_COLOR);

	// draw spheres
	for (const Sphere& s : m_spheres)
	{
		DUC->drawSphere(s.pos, Vec3(s.radius));
	}

	// draw springs
	DUC->beginLine();
	for (const SphereSpring& s : m_springs)
	{
		const Sphere& s1 = m_spheres[s.sphere1];
		const Sphere& s2 = m_spheres[s.sphere2];
		DUC->drawLine(s1.pos, SPRING_COLOR, s2.pos, SPRING_COLOR);
	}
	DUC->endLine();
}

void SphereSpringSystem::advanceLeapFrog(float timeStep, DrawingUtilitiesClass* DUC, bool onMouseDown, Vec3 mouseForce)
{
	computeForces(DUC);
	updatePosAndVel(timeStep);

	// update velocites from mouse interaction
	if (onMouseDown) {
		for (Sphere& s : m_spheres)
			s.vel += mouseForce * (timeStep / m_mass);
	}
}

void SphereSpringSystem::computeForces(DrawingUtilitiesClass* DUC)
{
	// reset forces
	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce = Vec3();
	}

	// repulsion forces
	handleCollisions();

	// spring forces	
	for (SphereSpring& spring : m_springs)
	{
		const int s1 = spring.sphere1;
		const int s2 = spring.sphere2;
		const Vec3 pos1 = m_spheres[s1].pos;
		const Vec3 pos2 = m_spheres[s2].pos;

		const Vec3 d = pos1 - pos2;
		const float l = norm(d);
		const float L = spring.initialLength;
		const float k = m_stiffness;

		const Vec3 f = d * (-k * (l - L) / l);

		m_spheres[s1].computedForce += f;
		m_spheres[s2].computedForce -= f;
	}

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

void SphereSpringSystem::updatePosAndVel(float timeStep)
{
	for (Sphere& sphere : m_spheres)
	{
		sphere.vel += sphere.computedForce * (timeStep / m_mass);
		sphere.pos += sphere.vel * timeStep;
	}
}

void SphereSpringSystem::handleCollisions()
{
	// collision with walls
	for (Sphere& sphere : m_spheres)
	{
		Vec3 pos = sphere.pos;
		Vec3 vel = sphere.vel;

		for (int f = 0; f < 6; f++)
		{
			float sign = (f % 2 == 0) ? -1.0f : 1.0f;
			if (sign * pos.value[f / 2] < -(BBOX_HALF_SIZE - sphere.radius))
			{
				pos.value[f / 2] = sign * -(BBOX_HALF_SIZE - sphere.radius - 0.001f);

				if (vel.value[f / 2] * sign < 0)
					vel.value[f / 2] *= -0.6; // bounce off walls, but lose some energy along the way
			}
		}

		sphere.pos = pos;
		sphere.vel = vel;
	}

	// collision between spheres
	m_uniformGrid.updateGrid(m_spheres);
	const std::unordered_set<std::pair<Sphere*, Sphere*>>& possiblyCollidingPairs = m_uniformGrid.computeCollisionPairs();

	for (auto& pair : possiblyCollidingPairs)
	{
		const double sqDist = pair.first->pos.squaredDistanceTo(pair.second->pos);
		const double diameter = pair.first->radius + pair.second->radius;

		// |d2-d1| < r1+r2 --> collision
		if (sqDist < diameter * diameter) {
			collisionResponse(*pair.first, *pair.second);
		}
	}
}

void SphereSpringSystem::collisionResponse(Sphere & sphere1, Sphere & sphere2)
{
	Vec3 n = sphere1.pos - sphere2.pos;
	const double dist = norm(n);
	const double diameter = sphere1.radius + sphere2.radius;

	const double lambda = 250.0f;

	// TODO maybe add kernel again?
	const double f = lambda * (1 - (dist / diameter));

	normalize(n); // unit length!!

	// TODO adjust response distribution according to sphere radius/mass
	sphere1.computedForce += f * n;
	sphere2.computedForce -= f * n;
}
