#include "Asteroid.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void Asteroid::Load()
{

}

void Asteroid::Update(float deltaTime)
{
	if (isActive)
	{
		mPosition += mVelocity * deltaTime;

		float width = (float)GraphicsSystem::Get()->GetBackBufferWidth();
		float height = (float)GraphicsSystem::Get()->GetBackBufferHeight();

		if (mPosition.x + mRadius <= 0.0f)
			isActive = false;
		if (mPosition.x - mRadius > width)
			isActive = false;
		if (mPosition.y + mRadius <= 0.0f)
			isActive = false;
		if (mPosition.y - mRadius > height)
			isActive = false;
	}
}

void Asteroid::Render()
{
	if (isActive)
		SimpleDraw::AddScreenCircle(mPosition, mRadius, Colors::DarkSlateGray);
}

void Asteroid::Spawn(Vector2 position, Type type)
{
	float speed = 100.0f;
	switch (type)
	{
	case Asteroid::Large:
		mRadius = 100.0f;
		speed = 100.0f;
		break;
	case Asteroid::Medium:
		mRadius = 50.0f;
		speed = 250.0f;
		break;
	case Asteroid::Small:
		mRadius = 25.0f;
		speed = 350.0f;
		break;
	default:
		mRadius = 50.0f;
		speed = 250.0f;
		break;
	}
	mType = type;
	isActive = true;

	mVelocity = Math::RandomVector2({ -1.0f ,-1.0f }, { 1.0f,1.0f }) * speed;
	mPosition = position;
}

