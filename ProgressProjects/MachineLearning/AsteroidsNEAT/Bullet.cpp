#include "Bullet.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Bullet::Update(float deltaTime)
{
	if (isActive)
	{
		mPosition += mVelocity * mSpeed *deltaTime;

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

void Bullet::Render()
{
	if (isActive)
		SimpleDraw::AddScreenCircle(mPosition, mRadius, Colors::AliceBlue);
}

void Bullet::Fire(const Angazi::Math::Vector2& velocity, const Angazi::Math::Vector2& position)
{
	isActive = true;
	mVelocity = velocity;
	mPosition = position;
}

