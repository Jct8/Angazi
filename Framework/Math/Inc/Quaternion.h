#pragma once

namespace Angazi::Math
{
	struct Quaternion
	{
		float x, y, z, w;

		Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Quaternion(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w) {}

		static const Quaternion Identity;

		const Quaternion operator-() const
		{
			return Quaternion(-x, -y, -z, -w);
		}
		const Quaternion operator+(const Quaternion &rhs) const
		{
			return Quaternion();
		}
		const Quaternion operator-(const Quaternion &lhs) const
		{
			return Quaternion();
		}
		const Quaternion operator*(const Quaternion &q) const
		{
			return Quaternion();
		}
	};
}