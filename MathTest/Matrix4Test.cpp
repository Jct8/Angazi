#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Angazi::Math;

namespace MathTest
{
	TEST_CLASS(Matrix4Test)
	{
	public:
		TEST_METHOD(Multiplication)
		{
			Matrix4 a{ 
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f
			};

			Matrix4 indentity = Matrix4::Identity;
			
			Matrix4 b = a * indentity;

			Assert::AreEqual(b._11, 1.0f);
			Assert::AreEqual(b._12, 2.0f);
			Assert::AreEqual(b._13, 3.0f);
			Assert::AreEqual(b._14, 4.0f);

			Assert::AreEqual(b._21, 1.0f);
			Assert::AreEqual(b._22, 2.0f);
			Assert::AreEqual(b._23, 3.0f);
			Assert::AreEqual(b._24, 4.0f);

			Assert::AreEqual(b._31, 1.0f);
			Assert::AreEqual(b._32, 2.0f);
			Assert::AreEqual(b._33, 3.0f);
			Assert::AreEqual(b._34, 4.0f);

			Assert::AreEqual(b._41, 1.0f);
			Assert::AreEqual(b._42, 2.0f);
			Assert::AreEqual(b._43, 3.0f);
			Assert::AreEqual(b._44, 4.0f);

		}
		TEST_METHOD(Translation)
		{
			Matrix4 a = Matrix4::Translation(Vector3{2.0f,3.0f,4.0f});

			Assert::AreEqual(a._11, 1.0f);
			Assert::AreEqual(a._12, 0.0f);
			Assert::AreEqual(a._13, 0.0f);
			Assert::AreEqual(a._14, 0.0f);

			Assert::AreEqual(a._21, 0.0f);
			Assert::AreEqual(a._22, 1.0f);
			Assert::AreEqual(a._23, 0.0f);
			Assert::AreEqual(a._24, 0.0f);

			Assert::AreEqual(a._31, 0.0f);
			Assert::AreEqual(a._32, 0.0f);
			Assert::AreEqual(a._33, 1.0f);
			Assert::AreEqual(a._34, 0.0f);

			Assert::AreEqual(a._41, 2.0f);
			Assert::AreEqual(a._42, 3.0f);
			Assert::AreEqual(a._43, 4.0f);
			Assert::AreEqual(a._44, 1.0f);

		}
		TEST_METHOD(RotationX)
		{
			Matrix4 a = Matrix4::RotationX(0.0f);

			Assert::AreEqual(a._11, 1.0f);
			Assert::AreEqual(a._12, 0.0f);
			Assert::AreEqual(a._13, 0.0f);
			Assert::AreEqual(a._14, 0.0f);

			Assert::AreEqual(a._21, 0.0f);
			Assert::AreEqual(a._22, 1.0f);
			Assert::AreEqual(a._23, 0.0f);
			Assert::AreEqual(a._24, 0.0f);

			Assert::AreEqual(a._31, 0.0f);
			Assert::AreEqual(a._32, 0.0f);
			Assert::AreEqual(a._33, 1.0f);
			Assert::AreEqual(a._34, 0.0f);

			Assert::AreEqual(a._41, 0.0f);
			Assert::AreEqual(a._42, 0.0f);
			Assert::AreEqual(a._43, 0.0f);
			Assert::AreEqual(a._44, 1.0f);

		}
		TEST_METHOD(RotationY)
		{
			Matrix4 a = Matrix4::RotationY(0.0f);

			Assert::AreEqual(a._11, 1.0f);
			Assert::AreEqual(a._12, 0.0f);
			Assert::AreEqual(a._13, 0.0f);
			Assert::AreEqual(a._14, 0.0f);

			Assert::AreEqual(a._21, 0.0f);
			Assert::AreEqual(a._22, 1.0f);
			Assert::AreEqual(a._23, 0.0f);
			Assert::AreEqual(a._24, 0.0f);

			Assert::AreEqual(a._31, 0.0f);
			Assert::AreEqual(a._32, 0.0f);
			Assert::AreEqual(a._33, 1.0f);
			Assert::AreEqual(a._34, 0.0f);

			Assert::AreEqual(a._41, 0.0f);
			Assert::AreEqual(a._42, 0.0f);
			Assert::AreEqual(a._43, 0.0f);
			Assert::AreEqual(a._44, 1.0f);

		}
		TEST_METHOD(RotationZ)
		{
			Matrix4 a = Matrix4::RotationZ(0.0f);

			Assert::AreEqual(a._11, 1.0f);
			Assert::AreEqual(a._12, 0.0f);
			Assert::AreEqual(a._13, 0.0f);
			Assert::AreEqual(a._14, 0.0f);

			Assert::AreEqual(a._21, 0.0f);
			Assert::AreEqual(a._22, 1.0f);
			Assert::AreEqual(a._23, 0.0f);
			Assert::AreEqual(a._24, 0.0f);

			Assert::AreEqual(a._31, 0.0f);
			Assert::AreEqual(a._32, 0.0f);
			Assert::AreEqual(a._33, 1.0f);
			Assert::AreEqual(a._34, 0.0f);

			Assert::AreEqual(a._41, 0.0f);
			Assert::AreEqual(a._42, 0.0f);
			Assert::AreEqual(a._43, 0.0f);
			Assert::AreEqual(a._44, 1.0f);

		}
		TEST_METHOD(Scaling)
		{
			Matrix4 a = Matrix4::Scaling(2.0f);

			Assert::AreEqual(a._11, 2.0f);
			Assert::AreEqual(a._12, 0.0f);
			Assert::AreEqual(a._13, 0.0f);
			Assert::AreEqual(a._14, 0.0f);

			Assert::AreEqual(a._21, 0.0f);
			Assert::AreEqual(a._22, 2.0f);
			Assert::AreEqual(a._23, 0.0f);
			Assert::AreEqual(a._24, 0.0f);

			Assert::AreEqual(a._31, 0.0f);
			Assert::AreEqual(a._32, 0.0f);
			Assert::AreEqual(a._33, 2.0f);
			Assert::AreEqual(a._34, 0.0f);

			Assert::AreEqual(a._41, 0.0f);
			Assert::AreEqual(a._42, 0.0f);
			Assert::AreEqual(a._43, 0.0f);
			Assert::AreEqual(a._44, 1.0f);

		}
	};
}