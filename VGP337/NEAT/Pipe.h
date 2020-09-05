#pragma once

#include <Angazi/Inc/Angazi.h>

class Pipe
{
public:
	void Update(float deltaTime);
	void Render();

	void Spawn(float gapSize);

	Angazi::Math::Rect GetTopRect() const;
	Angazi::Math::Rect GetBottomRect() const;

private:
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mVelocity;
	float mGapSize;
};