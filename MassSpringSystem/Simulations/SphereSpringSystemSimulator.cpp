#include "SphereSpringSystemSimulator.h"

SphereSpringSystemSimulator::SphereSpringSystemSimulator()
{
	// TODO testing values
	m_stiffness = 0.2;
	m_damping = 0.1;
	m_mass = 0.2;
	m_gravity = Vec3(0, -3, 0);
	const int gridCells = 15;
	const int cellCapacity = 50;

	m_SphereSpringSystem = new SphereSpringSystem(m_stiffness, m_damping, m_mass, m_gravity, gridCells, cellCapacity);

	// build tower
	buildTower(Vec3(0, -BBOX_HALF_SIZE, 0), Vec3(1, BBOX_HALF_SIZE, 1));

	// FIXME this spring moves more towards point p1
	const int p0 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(-1, 2, 1), Vec3(10, -10, -10));
	const int p1 = m_SphereSpringSystem->addSphere(BBOX_HALF_SIZE * Vec3(+1, 2, 1), Vec3(10, -10, -10));
	m_SphereSpringSystem->addSpring(p0, p1, 1.0);
}

void SphereSpringSystemSimulator::reset()
{
}

const char * SphereSpringSystemSimulator::getTestCasesStr()
{
	return "";
}

void SphereSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
}

void SphereSpringSystemSimulator::notifyCaseChanged(int testCase)
{
}

void SphereSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void SphereSpringSystemSimulator::simulateTimestep(float timeStep)
{
	m_SphereSpringSystem->advanceLeapFrog(timeStep);
}

void SphereSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	// draw walls
	const Vec3 wallColor = Vec3(1, 1, 1);
	DUC->beginLine();
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);

	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, -1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, -1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, -1), wallColor);

	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(-1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(-1, +1, +1), wallColor);
	DUC->drawLine(BBOX_HALF_SIZE * Vec3(+1, -1, +1), wallColor, BBOX_HALF_SIZE * Vec3(+1, +1, +1), wallColor);
	DUC->endLine();

	// draw sphere spring system
	m_SphereSpringSystem->draw(DUC);
}

void SphereSpringSystemSimulator::onClick(int x, int y)
{
}

void SphereSpringSystemSimulator::onLeftMouseRelease()
{
}

void SphereSpringSystemSimulator::onMouse(int x, int y)
{
}

void SphereSpringSystemSimulator::buildTower(Vec3 pos, Vec3 size)
{
	for (float y = 0; y < size.y; y += 2 * SPHERE_RADIUS) {
		const int p0 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, -size.z / 2), Vec3());
		const int p1 = m_SphereSpringSystem->addSphere(pos + Vec3(-size.x / 2, y, +size.z / 2), Vec3());
		const int p2 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, -size.z / 2), Vec3());
		const int p3 = m_SphereSpringSystem->addSphere(pos + Vec3(+size.x / 2, y, +size.z / 2), Vec3());

		for (int a = p0; a < p3; a++) {
			for (int b = a + 1; b <= p3; b++) {
				m_SphereSpringSystem->addSpring(a, b);
			}
		}
	}
}
