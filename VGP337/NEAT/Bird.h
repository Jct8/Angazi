#pragma once

#include <Angazi/Inc/Angazi.h>

class Bird
{
public:
	void Update(float deltaTime);
	void Render();

	void Flap();

	void SetPosition(const Angazi::Math::Vector2& pos) { mPosition = pos; }
	const Angazi::Math::Vector2& GetPosition() const { return mPosition; }

private:
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mVelocity;
	float mRadius = 30.0f;
};