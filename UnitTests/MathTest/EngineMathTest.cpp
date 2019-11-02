#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Angazi::Math;

namespace MathTest
{
	TEST_CLASS(EngineMathTest)
	{
	public:
		TEST_METHOD(DotProd)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 1.0f,2.0f,3.0f };
			float prod = Dot(a, b);

			Assert::AreEqual(prod, 14.0f);
		}
		TEST_METHOD(MagnitudeSqruared)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			float prod = MagnitudeSqr(a);

			Assert::AreEqual(prod, 14.0f);
		}
		TEST_METHOD(MagintudeTest)
		{
			Vector3 a{ 1.0f,2.0f,2.0f };
			float prod = Magnitude(a);

			Assert::AreEqual(prod, 3.0f);
		}
		TEST_METHOD(NormalizeTest)
		{
			Vector3 a{ 2.0f,2.0f,2.0f };
			Vector3 b = Normalize(a);

			float num = 1.0f / sqrt(3.0f);
			Assert::AreEqual(b.x, num);
			Assert::AreEqual(b.y, num);
			Assert::AreEqual(b.z, num);
		}
		TEST_METHOD(CrossProd)
		{
			Vector3 a{ 3.0f,3.0f,3.0f };
			Vector3 b{ 1.0f,2.0f,3.0f };
			Vector3 prod = Cross(a, b);

			Assert::AreEqual(prod.x, 3.0f);
			Assert::AreEqual(prod.y,-6.0f);
			Assert::AreEqual(prod.z, 3.0f);
		}
		TEST_METHOD(TransposeTest)
		{
			Matrix4 a{
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f,
				1.0f,2.0f,3.0f,4.0f
			};
			Matrix4 aT = Transpose(a);

			Assert::AreEqual(aT._11, 1.0f);
			Assert::AreEqual(aT._12, 1.0f);
			Assert::AreEqual(aT._13, 1.0f);
			Assert::AreEqual(aT._14, 1.0f);

			Assert::AreEqual(aT._21, 2.0f);
			Assert::AreEqual(aT._22, 2.0f);
			Assert::AreEqual(aT._23, 2.0f);
			Assert::AreEqual(aT._24, 2.0f);

			Assert::AreEqual(aT._31, 3.0f);
			Assert::AreEqual(aT._32, 3.0f);
			Assert::AreEqual(aT._33, 3.0f);
			Assert::AreEqual(aT._34, 3.0f);

			Assert::AreEqual(aT._41, 4.0f);
			Assert::AreEqual(aT._42, 4.0f);
			Assert::AreEqual(aT._43, 4.0f);
			Assert::AreEqual(aT._44, 4.0f);
		}

		TEST_METHOD(DeterminantTest)
		{
			Matrix4 a{
				5.0f,4.0f,3.0f,2.0f,
				1.0f,5.0f,4.0f,3.0f,
				2.0f,1.0f,5.0f,4.0f,
				3.0f,2.0f,1.0f,5.0f
			};
			float det = Determinant(a);

			Assert::AreEqual(det, 400.0f );
		}
		TEST_METHOD(InverseTest)
		{
			Matrix4 a{
				1.0f,1.0f,1.0f,-1.0f,
				1.0f,1.0f,-1.0f,1.0f,
				1.0f,-1.0f,1.0f,1.0f,
				-1.0f,1.0f,1.0f,1.0f
			};
			Matrix4 aInv = Inverse(a);

			Assert::AreEqual(aInv._11, 0.25f);
			Assert::AreEqual(aInv._12, 0.25f);
			Assert::AreEqual(aInv._13, 0.25f);
			Assert::AreEqual(aInv._14, -0.25f);

			Assert::AreEqual(aInv._21, 0.25f);
			Assert::AreEqual(aInv._22, 0.25f);
			Assert::AreEqual(aInv._23, -0.25f);
			Assert::AreEqual(aInv._24, 0.25f);

			Assert::AreEqual(aInv._31, 0.25f);
			Assert::AreEqual(aInv._32, -0.25f);
			Assert::AreEqual(aInv._33, 0.25f);
			Assert::AreEqual(aInv._34, 0.25f);

			Assert::AreEqual(aInv._41, -0.25f);
			Assert::AreEqual(aInv._42, 0.25f);
			Assert::AreEqual(aInv._43, 0.25f);
			Assert::AreEqual(aInv._44, 0.25f);
		}

		TEST_METHOD(TransformCoordTest)
		{
			Matrix4 a{
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f
			};
			Vector3 b{ 1.0f,2.0f,3.0f };

			Vector3 vec = TransformCoord(b,a);

			Assert::AreEqual(vec.x, 7.0f);
			Assert::AreEqual(vec.y, 7.0f);
			Assert::AreEqual(vec.z, 7.0f);
		}

		TEST_METHOD(TransformNormalTest)
		{
			Matrix4 a{
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f
			};
			Vector3 b{ 1.0f,2.0f,3.0f };

			Vector3 vec = TransformNormal(b, a);

			Assert::AreEqual(vec.x, 6.0f);
			Assert::AreEqual(vec.y, 6.0f);
			Assert::AreEqual(vec.z, 6.0f);

		}
		
	};
}