#pragma once

namespace Angazi::Math
{
	struct Matrix3
	{
		union
		{
			struct 
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			std::array<float, 9> v;

		};

		const static Matrix3 Identity;

		constexpr Matrix3 operator-() const
		{
			return Matrix3
			{
				-_11 , -_12 , -_13 ,
				-_21 , -_22 , -_23 ,
				-_31 , -_32 , -_33
			};
		}

		constexpr Matrix3 operator+(const Matrix3& rhs) const
		{
			return Matrix3
			{
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33
			};
		}
		constexpr Matrix3 operator-(const Matrix3& rhs) const
		{
			return Matrix3
			{
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, 
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, 
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33
			};
		}

		constexpr Matrix3 operator*(const Matrix3 &m) const
		{
			return
			{
				(_11 * m._11) + (_12 * m._21) + (_13 * m._31),
				(_11 * m._12) + (_12 * m._22) + (_13 * m._32),
				(_11 * m._13) + (_12 * m._23) + (_13 * m._33),

				(_21 * m._11) + (_22 * m._21) + (_23 * m._31),
				(_21 * m._12) + (_22 * m._22) + (_23 * m._32),
				(_21 * m._13) + (_22 * m._23) + (_23 * m._33),

				(_31 * m._11) + (_32 * m._21) + (_33 * m._31),
				(_31 * m._12) + (_32 * m._22) + (_33 * m._32),
				(_31 * m._13) + (_32 * m._23) + (_33 * m._33)
			};
		}
		constexpr Matrix3 operator*(float f) const
		{
			return Matrix3
			{
				_11 * f, _12 * f, _13 * f,
				_21 * f, _22 * f, _23 * f,
				_31 * f, _32 * f, _33 * f,
			};
		}

		constexpr Matrix3 operator/(float s) const
		{
			return Matrix3
			{
				_11 / s, _12 / s, _13 / s,
				_21 / s, _22 / s, _23 / s,
				_31 / s, _32 / s, _33 / s,
			};
		}
		constexpr Matrix3 operator+=(const Matrix3& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33;
			return *this;
		}
		static Matrix3 Translation(float x, float y)
		{
			return
			{
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				x , y , 1.0f
			};
		}
		static Matrix3 Scaling(float scale)
		{
			return
			{
				scale, 0.0f , 0.0f ,
				0.0f , scale, 0.0f ,
				0.0f , 0.0f , 1.0f
			};
		}

	};
}