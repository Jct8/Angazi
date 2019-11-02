#pragma once
#pragma once

namespace Angazi::Math
{
	struct Matrix4
	{
		// row + column subscripts
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

		/*Matrix4(float _11,float _12,float _13,float _14,
				float _21,float _22,float _23,float _24,
				float _31,float _32,float _33,float _34,
				float _41,float _42,float _43,float _44)
			:_11(_11), _12(_12), _13(_13), _14(_14),
			 _21(_21), _22(_22), _23(_23), _24(_24),
			 _31(_31), _32(_32), _33(_33), _34(_34),
			 _41(_41), _42(_42), _43(_43), _44(_44)
		{

		}*/

		const static Matrix4 Identity;

		constexpr Matrix4 operator*(const Matrix4 &m) const
		{
			return 
			{
				(_11 * m._11) + (_12 * m._21) + (_13 * m._31) + (_14 * m._41),
				(_11 * m._12) + (_22 * m._22) + (_13 * m._32) + (_14 * m._42),
				(_11 * m._13) + (_12 * m._23) + (_13 * m._33) + (_14 * m._43),
				(_11 * m._14) + (_12 * m._24) + (_13 * m._34) + (_14 * m._44),

				(_21 * m._11) + (_22 * m._21) + (_23 * m._31) + (_24 * m._41),
				(_21 * m._12) + (_22 * m._22) + (_23 * m._32) + (_24 * m._42),
				(_21 * m._13) + (_22 * m._23) + (_23 * m._33) + (_24 * m._43),
				(_21 * m._14) + (_22 * m._24) + (_23 * m._34) + (_24 * m._44),

				(_31 * m._11) + (_32 * m._21) + (_33 * m._31) + (_34 * m._41),
				(_31 * m._12) + (_32 * m._22) + (_33 * m._32) + (_34 * m._42),
				(_31 * m._13) + (_32 * m._23) + (_33 * m._33) + (_34 * m._43),
				(_31 * m._14) + (_32 * m._24) + (_33 * m._34) + (_34 * m._44),

				(_41 * m._11) + (_42 * m._21) + (_43 * m._31) + (_44 * m._41),
				(_41 * m._12) + (_42 * m._22) + (_43 * m._32) + (_44 * m._42),
				(_41 * m._13) + (_42 * m._23) + (_43 * m._33) + (_44 * m._43),
				(_41 * m._14) + (_42 * m._24) + (_43 * m._34) + (_44 * m._44)
			};
		}

		static Matrix4 Translation(const Vector3 & d)
		{
			return
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				d.x , d.y , d.z , 1.0f
			};
		}

		static Matrix4 RotationX(float radian)
		{
			return
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cosf(radian), sinf(radian), 0.0f,
				0.0f, -sinf(radian), cosf(radian), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		static Matrix4 RotationY(float radian)
		{
			return
			{
				cosf(radian), 0.0f, -sinf(radian), 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sinf(radian), 0.0f, cosf(radian), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		static Matrix4 RotationZ(float radian)
		{
			return
			{
				cosf(radian), sinf(radian),  0.0f, 0.0f,
				-sinf(radian), cosf(radian), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4 RotationAxis(const Vector3 & axis, float radian)
		{
			float cos = cosf(radian);
			float sin = sinf(radian);
			float wx = axis.x;
			float wy = axis.y;
			float wz = axis.z;

			return
			{
				/*cos+wx*wx*(1- cos)    , wx*wy*(1-cos) - wz*sin, wy*sin + wx*wz*(1 - cos), 0.0f,
				wz*sin + wx*wy*(1-cos), cos+wy*wy*(1-cos)     , -wx*sin+wy*wz*(1-cos)   , 0.0f,
				-wy*sin*wx*wz*(1-cos) , wx*sin+wy*wz*(1-cos)  , cos+wz*wz*(1-cos)       , 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f*/
				//Transpose
				cos + wx * wx*(1 - cos)   , wz*sin + wx * wy*(1 - cos)  , -wy * sin*wx*wz*(1 - cos) , 0.0f,
				wx*wy*(1 - cos) - wz * sin, cos + wy * wy*(1 - cos)     , wx*sin + wy * wz*(1 - cos), 0.0f,
				wy*sin + wx * wz*(1 - cos),-wx * sin + wy * wz*(1 - cos), cos + wz * wz*(1 - cos)   , 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		static Matrix4 Scaling(float scale)
		{
			return
			{
				scale, 0.0f , 0.0f , 0.0f,
				0.0f , scale, 0.0f , 0.0f,
				0.0f , 0.0f , scale, 0.0f,
				0.0f , 0.0f , 0.0f , 1.0f
			};
		}

	};
	 
}