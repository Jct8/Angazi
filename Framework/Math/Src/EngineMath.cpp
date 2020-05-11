#include "Precompiled.h"
#include "EngineMath.h"

using namespace Angazi::Math;

// Vector2 Definition
const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f };
const Vector2 Vector2::XAxis{ 1.0f , 0.0f };
const Vector2 Vector2::YAxis{ 0.0f , 1.0f };

// Vector3 Definition
const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ 1.0f , 0.0f, 0.0f };
const Vector3 Vector3::YAxis{ 0.0f , 1.0f, 0.0f };
const Vector3 Vector3::ZAxis{ 0.0f , 0.0f, 1.0f };

// Vector4 Definition
const Vector4 Vector4::Zero{ 0.0f };
const Vector4 Vector4::One{ 1.0f };
const Vector4 Vector4::XAxis{ 1.0f , 0.0f, 0.0f, 0.0f };
const Vector4 Vector4::YAxis{ 0.0f , 1.0f, 0.0f, 0.0f };
const Vector4 Vector4::ZAxis{ 0.0f , 0.0f, 1.0f, 0.0f };
const Vector4 Vector4::WAxis{ 0.0f , 0.0f, 0.0f, 1.0f };

// Quaternion Definition
const Quaternion Quaternion::Zero	 { 0.0f , 0.0f , 0.0f , 0.0f};
const Quaternion Quaternion::Identity{ 0.0f , 0.0f , 0.0f , 1.0f};

const Matrix4 Matrix4::Identity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const Matrix3 Matrix3::Identity
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};




bool Angazi::Math::PointInRect(const Vector2& point, const Rect& rect)
{
	return !(point.x < rect.left || point.x > rect.right || point.y < rect.top || point.y > rect.bottom);
}
bool Angazi::Math::PointInCircle(const Vector2& point, const Circle& circle)
{
	float distance = DistanceSqr(point, circle.center);
	return distance < circle.radius * circle.radius;
}

bool Angazi::Math::Intersect(const LineSegment& a, const LineSegment& b)
{
	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}
bool Angazi::Math::Intersect(const Circle& c0, const Circle& c1)
{
	float combinedRadius = c0.radius + c1.radius;
	float distanceSquared = DistanceSqr(c0.center, c1.center);
	return distanceSquared < combinedRadius*combinedRadius;
}
bool Angazi::Math::Intersect(const Rect& r0, const Rect& r1)
{
	return !(r0.right < r1.left || r0.left > r1.right || r0.bottom < r1.top || r0.top > r1.bottom);
}

bool Angazi::Math::Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = Magnitude(startToEnd);
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Dot(startToCenter, dir);
	Vector2 point;
	if (projection > len)
	{
		point = l.to;
	}
	else if (projection < 0.0f)
	{
		point = l.from;
	}
	else
	{
		point = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	if (!PointInCircle(point, c))
		return false;

	if (closestPoint)
		*closestPoint = point;

	return true;
}
bool Angazi::Math::Intersect(const LineSegment& l, const Circle& c)
{
	return Intersect(c, l);
}

bool Angazi::Math::Intersect(const Circle& c, const Rect& r)
{
	return Intersect(r, c);
}
bool Angazi::Math::Intersect(const Rect& r, const Circle& c)
{
	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.left, r.right);
	closestPoint.y = Clamp(c.center.y, r.top, r.bottom);

	const float distance = Distance(closestPoint, c.center);
	return !(distance > c.radius);
}

Quaternion Angazi::Math::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float dot = Dot(q1, q2);
	Quaternion newQ2 = q2;
	
	// Determine the direction of the rotation.
	if (dot < 0.0f)
	{
		dot = -dot;
		newQ2 = -q2;
	}
	else if (dot > 0.999f)
		return Normalize(Lerp(q1, newQ2, t));
	float theta = acosf(dot);
	float c1 = sinf(theta*(1.0f - t)) / sinf(theta);
	float c2 = sinf(theta*t) / sinf(theta);
	return q1*c1 + newQ2 *c2;
}
Quaternion Quaternion::RotationAxis(const Vector3& axis, float radian)
{
	const Vector3 a = Normalize(axis);
	return Quaternion
	(
		a.x* sinf(radian / 2.0f),
		a.y* sinf(radian / 2.0f),
		a.z* sinf(radian / 2.0f),
		cosf(radian / 2.0f)
	);
}
Quaternion Quaternion::RotationMatrix(const Matrix4& input)
{
	Matrix4 m = Transpose(input);
	float qw, qx, qy, qz;

	float tr = m._11 + m._22 + m._33;

	if (tr > 0) 
	{
		float w = sqrt(tr + 1.0f) * 2.0f; 
		qw = 0.25f * w;
		qx = (m._32 - m._23) / w;
		qy = (m._13 - m._31) / w;
		qz = (m._21 - m._12) / w;
	}
	else if ((m._11 > m._22)&(m._11 > m._33)) 
	{
		float w = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
		qw = (m._32 - m._23) / w;
		qx = 0.25f * w;
		qy = (m._12 + m._21) / w;
		qz = (m._13 + m._31) / w;
	}
	else if (m._22 > m._33) 
	{
		float w = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
		qw = (m._13 - m._31) / w;
		qx = (m._12 + m._21) / w;
		qy = 0.25f * w;
		qz = (m._23 + m._32) / w;
	}
	else 
	{
		float w = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
		qw = (m._21 - m._12) / w;
		qx = (m._13 + m._31) / w;
		qy = (m._23 + m._32) / w;
		qz = 0.25f * w;
	}
	return Quaternion(qx, qy, qz, qw);
}
Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to)
{
	Vector3 a = Cross(from, to);
	float w = Sqrt(Magnitude(from)* Magnitude(from)
		* Magnitude(to) * Magnitude(to)) + Dot(from, to);
	return Normalize(Quaternion(a.x, a.y, a.z, w));
}
Quaternion Quaternion::RotationLook(const Vector3& look, const Vector3& up)
{
	Vector3 z = Normalize(look);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	{
		x.x , x.y , x.z , 0.0f,
		y.x , y.y , y.z , 0.0f,
		z.x , z.y , z.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return RotationMatrix(mat);
}

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return Matrix4
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}
Matrix4 Matrix4::RotationAxis(const Vector3 & axis, float radian)
{
	float cos = cosf(radian);
	float sin = sinf(radian);
	Vector3 normalized = Normalize(axis);
	float wx = normalized.x;
	float wy = normalized.y;
	float wz = normalized.z;

	return
	{
		cos + wx * wx*(1.0f - cos)   , wz*sin + wx * wy*(1.0f - cos)  , -wy * sin + wx*wz*(1.0f - cos) , 0.0f,
		wx*wy*(1.0f - cos) - wz * sin, cos + wy * wy*(1.0f - cos)     , wx*sin + wy * wz*(1.0f - cos)  , 0.0f,
		wy*sin + wx * wz*(1.0f - cos),-wx * sin + wy * wz*(1.0f - cos), cos + wz * wz*(1.0f - cos)     , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}