#pragma once

namespace Angazi::Math
{
	struct Sphere
	{
		Vector3 center = Vector3::Zero;
		float radius = 1.0f;

		Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
		Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
		Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
	};
}