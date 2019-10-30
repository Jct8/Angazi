#pragma once

#include "Common.h"

#include "Vector3.h"
#include "Matrix4.h"

namespace Angazi::Math
{
	constexpr float Dot(const Vector3& a, const Vector3& b)
	{
		return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(Magnitude(v));
	}

	inline Vector3 Normalize(const Vector3 & v)
	{
		return v / Magnitude(v);
	}

	constexpr Vector3 Cross(const Vector3 &a, const Vector3 &b)
	{
		return { (a.y*b.z) - (b.y*a.z), -(a.x*b.z) + (b.x*a.z) , (a.x*b.y) - (b.x *a.y) };
	}

	constexpr Matrix4 Transpose(const Matrix4 &m)
	{
		return 
		{
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		};
	}

	constexpr float Determinant(const Matrix4 &m)
	{
		float det = 0.0f;
		return
		{
			(m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))))
			- (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))))
			+ (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))))
			- (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))))
		};
	}

	constexpr Matrix4 Inverse(const Matrix4 &m)
	{
		float determinant = Determinant(m);
		float inverseDet = 1.0f / determinant;
		Matrix4 adjointMatrix
		{
			inverseDet*(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			inverseDet*-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			inverseDet*(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			inverseDet*-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			inverseDet*-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			inverseDet*(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			inverseDet*-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			inverseDet*(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			inverseDet*(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			inverseDet*-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			inverseDet*(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			inverseDet*-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			inverseDet*-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			inverseDet*(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			inverseDet*-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			inverseDet*(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};

		return adjointMatrix;
	}

	constexpr Vector3 TransformCoord(const Vector3& v , const Matrix4 &m)
	{
		return 
		{ 
			(v.x * m._11) + (v.y * m._21) + (v.z * m._31) + m._41 ,
			(v.x * m._12) + (v.y * m._22) + (v.z * m._32) + m._42 ,
			(v.x * m._13) + (v.y * m._23) + (v.z * m._33) + m._43 
		};
	}
	
	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4 &m)
	{
		return
		{
			(v.x * m._11) + (v.y * m._21) + (v.z * m._31),
			(v.x * m._12) + (v.y * m._22) + (v.z * m._32),
			(v.x * m._13) + (v.y * m._23) + (v.z * m._33)
		};
	}


}