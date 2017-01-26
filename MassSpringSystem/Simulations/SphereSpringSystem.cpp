#include "SphereSpringSystem.h"

int SphereSpringSystem::addSphere(Vec3 pos, Vec3 vel, float radius, bool fixed)
{
	Sphere s;
	s.pos = pos;
	s.vel = vel;
	s.computedForce = Vec3();
	s.radius = radius;
	s.fixed = fixed;

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

		Vec3 color = SPRING_COLOR;
		if (m_bridgeBuilderView) {
			Vec3 colorGood = Vec3(0, 1, 0);
			Vec3 colorBad = Vec3(1, 0, 0);
			color = (colorBad - colorGood) * s.suicideProgress * 2 + colorGood;
		}
		DUC->drawLine(s1.pos, color, s2.pos, color);
	}
	DUC->endLine();
}

void SphereSpringSystem::advanceLeapFrog(float timeStep, DrawingUtilitiesClass* DUC, bool onMouseDown, Vec3 mouseForce)
{
	computeForces(DUC);
	updatePosAndVel(timeStep);
	handleWallCollisions();

	// update velocites from mouse interaction
	if (onMouseDown) {
		for (Sphere& s : m_spheres)
			s.vel += mouseForce * (timeStep / s.getMass());
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
	for(auto i=m_springs.begin(); i != m_springs.end();)
	{
		SphereSpring& spring = *i;
		Sphere& sphere1 = m_spheres[spring.sphere1];
		Sphere& sphere2 = m_spheres[spring.sphere2];

		const Vec3 d = sphere1.pos - sphere2.pos;
		const float l = norm(d);
		const float L = spring.initialLength;
		const float k = m_stiffness;

		const Vec3 f = d * (-k * (l - L) / l);

		const float normSquared = normNoSqrt(f);

		spring.suicideProgress = normSquared / (m_breakThreshold * m_breakThreshold);

		if (normSquared > m_breakThreshold * m_breakThreshold) {
			i = m_springs.erase(i);
		}
		else {
			const double m1 = pow(sphere1.radius, 3);
			const double m2 = pow(sphere2.radius, 3);

			sphere1.computedForce += f * m2 / m1;
			sphere2.computedForce -= f * m1 / m2;

			i++;	
		}
	}

	// Gravity & Damping forces
	Vec3 gravity = m_gravity;
	if (m_camRotDependentGravity)
	{
		// direction of gravity will be the bottom of the screen
		Mat4 viewMatrix(DUC->g_camera.GetViewMatrix());
		gravity = viewMatrix * gravity;
	}

	for (Sphere& sphere : m_spheres)
	{
		sphere.computedForce += gravity * sphere.getMass();
		sphere.computedForce += sphere.vel * -m_damping;
	}
}

void SphereSpringSystem::updatePosAndVel(float timeStep)
{
	for (Sphere& sphere : m_spheres)
	{
		if (sphere.fixed) continue;
		sphere.vel += sphere.computedForce * (timeStep / sphere.getMass());
		sphere.pos += sphere.vel * timeStep;
	}
}

void SphereSpringSystem::handleCollisions()
{
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

void SphereSpringSystem::handleWallCollisions()
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
}

void SphereSpringSystem::collisionResponse(Sphere & sphere1, Sphere & sphere2)
{
	Vec3 n = sphere1.pos - sphere2.pos;
	const double dist = norm(n);
	const double diameter = sphere1.radius + sphere2.radius;

	const double lambda = 1000.0f;

	// TODO maybe add kernel again?
	const double f = lambda * (1 - (dist / diameter));

	normalize(n); // unit length!!

	const double m1 = pow(sphere1.radius, 3);
	const double m2 = pow(sphere2.radius, 3);


	// colliding with a heavier object 2 results in a larger force on 1!
	sphere1.computedForce += f * m2 * n;
	sphere2.computedForce -= f * m1 * n;
}
