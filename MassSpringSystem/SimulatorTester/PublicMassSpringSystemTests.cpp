#include "CppUnitTest.h"
#include "MassSpringSystemSimulator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimulatorTester
{
	TEST_CLASS(PublicMassSpringSystemTests)
	{
	public:
		void setupBaseTest(MassSpringSystemSimulator * msss) {
			msss->m_iTestCase = 1;
			msss->setMass(10.0f);
			msss->setDampingFactor(0.0f);
			msss->setStiffness(40.0f);
			msss->applyExternalForce(Vec3(0.0f, 0.0f, 0.0f));// no gravity or other forces should be used!
			msss->addMassPoint(Vec3(0.0, 0.0f, 0), Vec3(-1.0, 0.0f, 0), false);
			msss->addMassPoint(Vec3(0.0, 2.0f, 0), Vec3(1.0, 0.0f, 0), false);
			msss->addSpring(0, 1, 1.0);
		}
		TEST_METHOD(TestNumberofMassPoints)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			Assert::AreEqual(2.0f, (float)msss->getNumberOfMassPoints(), 0.0001f, L"Number of Mass Points is not equal to 2", LINE_INFO());
			delete msss;
		}
		TEST_METHOD(TestNumberofSprings)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();;
			setupBaseTest(msss);
			Assert::AreEqual(1.0f, (float)msss->getNumberOfSprings(), 0.0001f, L"Number of Mass Points is not equal to 1", LINE_INFO());
			delete msss;
		}
		TEST_METHOD(TestPositionOfMassPointsInitially)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);

			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(2.0f, (float)msss->getPositionOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());
			delete msss;
		}


		TEST_METHOD(TestPositionOfMassPointsAfter1TimeStepMidPoint)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			msss->setIntegrator(MIDPOINT);
			msss->simulateTimestep(0.1);
			Assert::AreEqual(-0.1f, (float)msss->getPositionOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.02f, (float)msss->getPositionOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.1f, (float)msss->getPositionOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(1.98f, (float)msss->getPositionOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());
			delete msss;
		}

		TEST_METHOD(TestPositionOfMassPointsAfter1TimeStepEuler)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			msss->setIntegrator(EULER);
			//msss->notifyCaseChanged(1);
			msss->simulateTimestep(0.1);
			Assert::AreEqual(-0.1f, (float)msss->getPositionOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.1f, (float)msss->getPositionOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(2.0f, (float)msss->getPositionOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getPositionOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());

			delete msss;
		}



		TEST_METHOD(TestVelocityOfMassPointsInitially)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			Assert::AreEqual(-1.0f, (float)msss->getVelocityOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(1.0f, (float)msss->getVelocityOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());
			delete msss;
		}

		TEST_METHOD(TestVelocityOfMassPointsAfter1TimeStepEuler)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			msss->setIntegrator(EULER);
			msss->simulateTimestep(0.1);
			Assert::AreEqual(-1.0f, (float)msss->getVelocityOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.4f, (float)msss->getVelocityOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(1.0f, (float)msss->getVelocityOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(-0.4f, (float)msss->getVelocityOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());
			delete msss;
		}

		TEST_METHOD(TestVelocityOfMassPointsAfter1TimeStepMidPoint)
		{
			MassSpringSystemSimulator * msss = new MassSpringSystemSimulator();
			setupBaseTest(msss);
			msss->setIntegrator(MIDPOINT);
			msss->simulateTimestep(0.1);
			Assert::AreEqual(-0.979975f, (float)msss->getVelocityOfMassPoint(0).x, 0.0001f, L"Mass Point at index 0, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.400499f, (float)msss->getVelocityOfMassPoint(0).y, 0.0001f, L"Mass Point at index 0, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(0).z, 0.0001f, L"Mass Point at index 0, Z value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.979975f, (float)msss->getVelocityOfMassPoint(1).x, 0.0001f, L"Mass Point at index 1, X value is wrong !!", LINE_INFO());
			Assert::AreEqual(-0.400499f, (float)msss->getVelocityOfMassPoint(1).y, 0.0001f, L"Mass Point at index 1, Y value is wrong !!", LINE_INFO());
			Assert::AreEqual(0.0f, (float)msss->getVelocityOfMassPoint(1).z, 0.0001f, L"Mass Point at index 1, Z value is wrong !!", LINE_INFO());
			delete msss;
		}

	};
}