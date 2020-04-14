#pragma once

namespace Angazi::Math
{
	struct Matrix4;

	struct Quaternion
	{
		float x, y, z, w;

		Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Quaternion(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w) {}

		static const Quaternion Identity;
		static const Quaternion Zero;

		const Quaternion operator-()const						{ return Quaternion(-x, -y, -z, -w); }
		const Quaternion operator+(const Quaternion &rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		const Quaternion operator-(const Quaternion &rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		const Quaternion operator*(float value) const			{ return Quaternion(x * value, y * value, z * value, w * value); }
		const Quaternion operator/(float value) const			{ return Quaternion(x / value, y / value, z / value, w / value); }
		const Quaternion operator*(const Quaternion &q) const
		{
			return Quaternion
			(
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y - x * q.z + y * q.w + z * q.x,
				w * q.z + x * q.y - y * q.x + z * q.w,
				w * q.w - x * q.x - y * q.y - z * q.z
			);
		}

		const bool operator==(const Quaternion& q) const { return (x == q.x && y == q.y && z == q.z && w == q.w); }

		static Quaternion Inverse(const Quaternion& q) { return Quaternion(-q.x, -q.y, -q.z, q.w); }
		static Quaternion RotationAxis(const Vector3& axis, float radian);
		static Quaternion RotationMatrix(const Matrix4& m);
		static Quaternion RotationLook(const Vector3& look, const Vector3& up = Vector3::YAxis);
		static Quaternion RotationFromTo(const Vector3& from, const Vector3& to);
	};
}