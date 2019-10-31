#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Angazi::Math;

namespace MathTest
{		
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestStatics)
		{
			Vector3 zero = Vector3::Zero;
			Assert::AreEqual(zero.x, 0.0f);
			Assert::AreEqual(zero.y, 0.0f);
			Assert::AreEqual(zero.z, 0.0f);

			Vector3 one = Vector3::One;
			Assert::AreEqual(one.x, 1.0f);
			Assert::AreEqual(one.y, 1.0f);
			Assert::AreEqual(one.z, 1.0f);
		}
		TEST_METHOD(Additions)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			Vector3 c= a+b;

			Assert::AreEqual(c.x, 5.0f);
			Assert::AreEqual(c.y, 7.0f);
			Assert::AreEqual(c.z, 9.0f);

		}
		TEST_METHOD(Subtractions)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			Vector3 c = b-a;

			Assert::AreEqual(c.x, 3.0f);
			Assert::AreEqual(c.y, 3.0f);
			Assert::AreEqual(c.z, 3.0f);
		}
		TEST_METHOD(Multiplaction)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 c = a * 5;

			Assert::AreEqual(c.x, 5.0f);
			Assert::AreEqual(c.y, 10.0f);
			Assert::AreEqual(c.z, 15.0f);

		}
		TEST_METHOD(Division)
		{
			Vector3 a{ 10.0f,20.f,30.0f };
			Vector3 c = a / 5;

			Assert::AreEqual(c.x, 2.0f);
			Assert::AreEqual(c.y, 4.0f);
			Assert::AreEqual(c.z, 6.0f);
		}
		TEST_METHOD(AdditionAssignment)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			a += b;

			Assert::AreEqual(a.x, 5.0f);
			Assert::AreEqual(a.y, 7.0f);
			Assert::AreEqual(a.z, 9.0f);
		}
		TEST_METHOD(SubtractionAssignment)
		{
			Vector3 b{ 1.0f,2.0f,3.0f };
			Vector3 a{ 4.0f,5.0f,6.0f };
			a -= b;

			Assert::AreEqual(a.x, 3.0f);
			Assert::AreEqual(a.y, 3.0f);
			Assert::AreEqual(a.z, 3.0f);
		}
		TEST_METHOD(MultiplactionAssignment)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };

			a *= 2;

			Assert::AreEqual(a.x, 2.0f);
			Assert::AreEqual(a.y, 4.0f);
			Assert::AreEqual(a.z, 6.0f);
		}
		TEST_METHOD(DivisonAssignment)
		{
			Vector3 a{ 4.0f,2.0f,6.0f };

			a /= 2;

			Assert::AreEqual(a.x, 2.0f);
			Assert::AreEqual(a.y, 1.0f);
			Assert::AreEqual(a.z, 3.0f);
		}
	};
}