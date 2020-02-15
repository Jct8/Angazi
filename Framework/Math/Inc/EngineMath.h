#pragma once

#include "Common.h"

#include "Constants.h"
#include "Shapes.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Angazi::Math
{
	template <class T> constexpr T Min(T a, T b)						{ return (a < b) ? a : b;}
	template <class T> constexpr T Max(T a, T b)						{return (a > b) ? a : b;	}
	template <class T> constexpr T Clamp(T value, T min,T max)			{ return Max(min, Min(max, value));}
	template <class T> constexpr T Lerp(T a, T b, float t)				{	return a + (b - a) *t;}

	constexpr float Abs(float value)									{ return value >= 0.0f ? value : -value;}
	constexpr float Sign(float value)									{ return value >=0 ? value: -value; }
	constexpr float Sqr(float value)									{ return value * value; }
	inline float Sqrt(float value)										{ return sqrtf(value); }

	constexpr float MagnitudeSqr(const Vector2& v)						{ return (v.x*v.x) + (v.y*v.y); }
	constexpr float MagnitudeSqr(const Vector3& v)						{ return (v.x*v.x) + (v.y*v.y) + (v.z*v.z); }
	inline float Magnitude(const Vector2& v)							{ return sqrt(MagnitudeSqr(v)); }
	inline float Magnitude(const Vector3& v)							{ return sqrt(MagnitudeSqr(v)); }

	inline Vector3 Normalize(const Vector3 & v)							{ return v / Magnitude(v); }
	inline Vector2 Normalize(const Vector2 & v)							{ return v / Magnitude(v); }

	constexpr float DistanceSqr(const Vector2 &v1, const Vector2 &v2)	{ return MagnitudeSqr(v1 - v2); }
	constexpr float DistanceSqr(const Vector3 &v1, const Vector3 &v2)	{ return MagnitudeSqr(v1 - v2); }
	inline float Distance(const Vector3 &v1, const Vector3 &v2)			{ return Magnitude(v1 - v2); }
	inline float Distance(const Vector2 &v1, const Vector2 &v2)			{ return Magnitude(v1 - v2); }
	constexpr float Dot(const Vector2& a, const Vector2& b)				{ return (a.x*b.x) + (a.y*b.y); }
	constexpr float Dot(const Vector3& a, const Vector3& b)				{ return (a.x*b.x) + (a.y*b.y) + (a.z*b.z); }
	constexpr Vector3 Cross(const Vector3 &a, const Vector3 &b)			{ return { (a.y*b.z) - (b.y*a.z), -(a.x*b.z) + (b.x*a.z) , (a.x*b.y) - (b.x *a.y) }; }

	inline Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t)	{ v1 + ((v2 - v1)*t); }
	inline Vector2 Lerp(const Vector3& v1, const Vector3& v2, float t)	{ v1 + ((v2 - v1)*t); }

	constexpr float Determinant(const Matrix4 &m)
	{
		return
		{
			(m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))))
			- (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))))
			+ (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))))
			- (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))))
		};
	}
	constexpr Matrix4 Adjoint(const Matrix4& m)
	{
		Matrix4 adjointMatrix
		{
			(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23)))
			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),
			(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14)))
			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};
		return adjointMatrix;
	}

	constexpr Matrix4 Inverse(const Matrix4 &m)
	{
		float determinant = Determinant(m);
		float inverseDet = 1.0f / determinant;
		return Adjoint(m) * inverseDet;
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

	bool PointInRect(const Vector2& point, const Rect& rect);
	bool PointInCircle(const Vector2& point, const Circle& circle);

	bool Intersect(const LineSegment& a, const LineSegment& b);
	bool Intersect(const Circle& c0, const Circle& c1);
	bool Intersect(const Rect& r0, const Rect& r1);

	bool Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint = nullptr);
	bool Intersect(const LineSegment& l, const Circle& c);

	bool Intersect(const Circle& c, const Rect& r);
	bool Intersect(const Rect& r, const Circle& c);

} //namespace Angazi::Math