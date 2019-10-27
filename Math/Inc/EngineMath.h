#pragma once

#include "Common.h"

#include "Vector3.h"
#include "Matrix4.h"

namespace Angazi::Math
{
	constexpr float Dot(const Vector3& a, const Vector3& b)
	{
		return 0.0f;
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return 0.0f; // homework
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(Magnitude(v));
	}

	inline Vector3 Normalize(const Vector3 & v)
	{
		return 0.0f;
	}

	constexpr Vector3 Cross(const Vector3 &a, const Vector3 &b)
	{
		return 0.0f;
	}

	constexpr Matrix4 Transpose(const Matrix4 &m)
	{
		return {};
	}

	constexpr Matrix4 Determinant(const Matrix4 &m)
	{
		return {};
	}

	constexpr Matrix4 Inverse(const Matrix4 &m)
	{
		return {};
	}

	constexpr Vector3 TransformCoord(const Vector3& v , const Matrix4 &m)
	{
		return {};
	}
	
	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4 &m)
	{
		return {};
	}


}