#include "Pipe.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void Pipe::Load()
{
	mTextureId = TextureManager::Get()->Load("FlappyBird/pipe.png", false, true);
}

void Pipe::Update(float deltaTime)
{
	mPosition += mVelocity * deltaTime;
}

void Pipe::Render()
{
	auto topRect = GetTopRect();
	auto bottomRect = GetBottomRect();

	BatchRenderer::Get()->AddSprite(mTextureId, { topRect.left, topRect.bottom }, Pivot::BottomLeft, Flip::Vertical);
	BatchRenderer::Get()->AddSprite(mTextureId, { bottomRect.left, bottomRect.top }, Pivot::TopLeft);

	//SimpleDraw::AddScreenRect(GetTopRect(), Colors::Green);
	//SimpleDraw::AddScreenRect(GetBottomRect(), Colors::Green);
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
		-FLT_MAX,
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
		FLT_MAX,
	};
}