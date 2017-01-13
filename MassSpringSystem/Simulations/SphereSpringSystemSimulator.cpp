#include "SphereSpringSystemSimulator.h"

SphereSpringSystemSimulator::SphereSpringSystemSimulator()
{
	m_SphereSpringSystem = new SphereSpringSystem();

	// TODO testing
	int p0 = m_SphereSpringSystem->addSphere(Vec3(-1, 0, 0), Vec3());
	int p1 = m_SphereSpringSystem->addSphere(Vec3(+1, 0, 0), Vec3());
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
}

void SphereSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
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
