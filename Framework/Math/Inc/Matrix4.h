#pragma once

namespace Angazi::Math
{
	struct Matrix4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};

		const static Matrix4 Identity;

		constexpr Matrix4 operator-() const
		{
			return Matrix4
			{
				-_11 , -_12 , -_13 , -_14 ,
				-_21 , -_22 , -_23 , -_24 ,
				-_31 , -_32 , -_33 , -_34 ,
				-_41 , -_42 , -_43 , -_44
			};
		}
		constexpr Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4
			{
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44
			};
		}
		constexpr Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4
			{
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44
			};
		}

		constexpr Matrix4 operator*(const Matrix4 &m) const
		{
			return
			{
				(_11 * m._11) + (_12 * m._21) + (_13 * m._31) + (_14 * m._41),
				(_11 * m._12) + (_12 * m._22) + (_13 * m._32) + (_14 * m._42),
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
		constexpr Matrix4 operator*(float f) const
		{
			return Matrix4
			{
				_11 * f, _12 * f, _13 * f, _14 * f,
				_21 * f, _22 * f, _23 * f, _24 * f,
				_31 * f, _32 * f, _33 * f, _34 * f,
				_41 * f, _42 * f, _43 * f, _44 * f
			};
		}

		constexpr Matrix4 operator/(float s) const
		{
			return Matrix4
			{
				_11 / s, _12 / s, _13 / s, _14 / s,
				_21 / s, _22 / s, _23 / s, _24 / s,
				_31 / s, _32 / s, _33 / s, _34 / s,
				_41 / s, _42 / s, _43 / s, _44 / s
			};
		}
		constexpr Matrix4& operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}

		static Matrix4 Translation(float x, float y, float z)
		{
			return
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				x , y , z , 1.0f
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
		static Matrix4 RotationAxis(const Vector3 & axis, float radian);

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
		static Matrix4 Scaling(const Vector3 &scale)
		{
			return
			{
				scale.x, 0.0f   , 0.0f   , 0.0f,
				0.0f   , scale.y, 0.0f   , 0.0f,
				0.0f   , 0.0f   , scale.z, 0.0f,
				0.0f   , 0.0f   , 0.0f   , 1.0f
			};
		}

		static Matrix4 Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
		{
			Matrix4 rot = Matrix4::RotationX(rotation.x) * Matrix4::RotationY(rotation.y) * Matrix4::RotationZ(rotation.z);
			return rot * Matrix4::Scaling(scale) * Matrix4::Translation(position);
		}

		static Matrix4 RotationQuaternion(const Quaternion& q);
	};

}