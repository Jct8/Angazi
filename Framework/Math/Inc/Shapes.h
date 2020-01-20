#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace Angazi::Math
{
	struct LineSegment
	{
		Vector2 to;
		Vector2 from;

		constexpr LineSegment() :to(), from() {}
		constexpr LineSegment(float x0, float y0, float x1, float y1) :to(), from() {}
		constexpr LineSegment(Vector2 v0, Vector2 v1):to(v0) , from(v1) {}
	};

	struct Rect
	{
		float left;
		float top;
		float right;
		float bottom;

		constexpr Rect() : Rect{ 0.0f, 0.0f, 0.0f ,0.0f } {}
		constexpr Rect(float left, float top, float right, float bottom) : left{ left }, top{ top }, right{ right }, bottom{ bottom }{}
	};

	struct Circle
	{
		float radius;
		Vector2 center;

		Circle() : radius(1.0f), center(0.0f, 0.0f) {}
		Circle(const Vector2& center, float radius) : radius(radius), center(center) {}
		Circle(float x , float y, float radius) : radius(radius), center(x,y) {}
	};

	struct Sphere
	{
		float radius;
		Vector3 center;

		Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
		Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
		Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
	};
}