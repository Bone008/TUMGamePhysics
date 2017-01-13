#include "SphereSpringSystemSimulator.h"

SphereSpringSystemSimulator::SphereSpringSystemSimulator()
{
	// TODO testing values
	m_stiffness = 0.2;
	m_damping = 0.1;
	m_mass = 0.2;
	const int gridCells = 10;
	const int cellCapacity = 10;

	m_SphereSpringSystem = new SphereSpringSystem(m_stiffness, m_damping, m_mass, gridCells, cellCapacity);

	// TODO testing
	const int p0 = m_SphereSpringSystem->addSphere(Vec3(-1, 0, 0), Vec3(0, +1, 0));
	const int p1 = m_SphereSpringSystem->addSphere(Vec3(+1, 0, 0), Vec3(0, -1, 0));
	m_SphereSpringSystem->addSpring(p0, p1, 1.0);

	const int p2 = m_SphereSpringSystem->addSphere(Vec3(0, +1, 0), Vec3(-1, 0, 0));
	const int p3 = m_SphereSpringSystem->addSphere(Vec3(0, -1, 0), Vec3(+1, 0, 0));
	m_SphereSpringSystem->addSpring(p2, p3, 1.0);
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
