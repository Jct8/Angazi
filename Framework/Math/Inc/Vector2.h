#pragma once

namespace Angazi::Math
{
	struct Vector2
	{
		float x, y;

		constexpr Vector2() noexcept : Vector2{ 0.0f } {}
		constexpr Vector2(float f) noexcept : Vector2{ f , f } {}
		constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

		//Declaration
		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;

		constexpr Vector2 operator-() const { return { -x , -y }; }
		constexpr Vector2 operator+(const Vector2 &v) const { return { x + v.x , y + v.y }; }
		constexpr Vector2 operator-(const Vector2 &v) const { return { x - v.x , y - v.y }; }
		constexpr Vector2 operator*(float s) const { return { x*s , y*s }; }
		constexpr Vector2 operator/(float s) const { return { x / s, y / s }; }

		Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
		Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
		Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
		Vector2& operator/=(float s) { x /= s; y /= s; return *this; }
	};

}