#pragma once

#include <Angazi/Inc/Angazi.h>

class Bullet
{
public:
	void Update(float deltaTime);
	void Render();

	Angazi::Math::Circle GetBoundingCircle() const { return { mPosition, mRadius }; }

	void Fire(const Angazi::Math::Vector2& velocity, const Angazi::Math::Vector2& position);

	void SetActive(bool active) { isActive = active; }
	bool IsActive() const { return isActive; }

private:
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mVelocity;

	float mSpeed = 1000.0f;
	float mRadius = 1.0f;
	bool isActive = false;
};

