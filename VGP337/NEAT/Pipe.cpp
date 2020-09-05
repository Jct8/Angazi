#include "Pipe.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void Pipe::Update(float deltaTime)
{
	mPosition += mVelocity * deltaTime;
}

void Pipe::Render()
{
	SimpleDraw::AddScreenRect(GetTopRect(), Colors::Green);
	SimpleDraw::AddScreenRect(GetBottomRect(), Colors::Green);
}

void Pipe::Spawn(float gapSize)
{
	mGapSize = gapSize;
	mVelocity = Angazi::Math::Vector2{ -100.0f, 0.0f };
	mPosition.x = (float)GraphicsSystem::Get()->GetBackBufferWidth();
	mPosition.y = Angazi::Math::RandomFloat(gapSize, GraphicsSystem::Get()->GetBackBufferHeight() - gapSize);
}

Angazi::Math::Rect Pipe::GetTopRect() const
{
	return {
		mPosition.x,
		mPosition.y - mGapSize - GraphicsSystem::Get()->GetBackBufferHeight(),
		mPosition.x + mGapSize,
		mPosition.y - mGapSize,
	};
}

Angazi::Math::Rect Pipe::GetBottomRect() const
{
	return {
		mPosition.x,
		mPosition.y + mGapSize,
		mPosition.x + mGapSize,
		mPosition.y + mGapSize + GraphicsSystem::Get()->GetBackBufferWidth(),
	};
}