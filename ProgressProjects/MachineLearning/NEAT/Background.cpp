#include "Background.h"

using namespace Angazi::Graphics;

void Background::Load()
{
	mTextureId = TextureManager::Get()->Load("FlappyBird/bg.jpg");
	mOffset.x = (float)TextureManager::Get()->GetTexture(mTextureId)->GetWidth();
}

void Background::Update(float deltaTime)
{
	auto velocity = Angazi::Math::Vector2{ -100.0f, 0.0f };
	mPosition += velocity * deltaTime;
	if (mPosition.x <= -mOffset.x)
		mPosition.x += mOffset.x;
}

void Background::Render()
{
	BatchRenderer::Get()->AddSprite(mTextureId, mPosition, Pivot::TopLeft);
	BatchRenderer::Get()->AddSprite(mTextureId, mPosition + mOffset, Pivot::TopLeft);
}