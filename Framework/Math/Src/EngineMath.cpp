#include "Precompiled.h"
#include "EngineMath.h"

using namespace Angazi::Math;

//Definition
const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f };
const Vector2 Vector2::XAxis{ 1.0f , 0.0f };
const Vector2 Vector2::YAxis{ 0.0f , 1.0f };

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ 1.0f , 0.0f, 0.0f};
const Vector3 Vector3::YAxis{ 0.0f , 1.0f, 0.0f};
const Vector3 Vector3::ZAxis{ 0.0f , 0.0f, 1.0f};

//Declaration
const Vector4 Vector4::Zero{ 0.0f };
const Vector4 Vector4::One{ 1.0f };
const Vector4 Vector4::XAxis{ 1.0f , 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::YAxis{ 0.0f , 1.0f, 0.0f, 0.0f};
const Vector4 Vector4::ZAxis{ 0.0f , 0.0f, 1.0f, 0.0f };
const Vector4 Vector4::WAxis{ 0.0f , 0.0f, 0.0f, 1.0f};

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
