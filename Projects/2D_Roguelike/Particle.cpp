#include "Particle.h"
#include "Camera2D.h"

using namespace Angazi;

void Particle::Load(std::string fileName,int columns, int rows)
{
	mTexture = Graphics::TextureManager::Get()->Load(fileName.c_str());
	mDuration = 0.3f;
	mFrame = 0;
	mFrameCount = 6;
	mFrameTimer = mDuration/ mFrameCount;
	isActive = false;
	mColumns = columns;
	mRows = rows;
}

void Particle::Unload()
{
	mTexture = 0;
}

void Particle::Update(float deltaTime)
{
	if (!isActive)
		return;
	mFrameTimer -= deltaTime;
	if (mFrameTimer <= 0.0f)
	{
		mFrameTimer = mDuration / mFrameCount;
		mFrame = (mFrame + 1) % mFrameCount;
	}
}

void Particle::Render()
{
	if (!isActive)
		return;
	Math::Vector2 screenPos = Camera2D::Get().ConvertToScreenPosition(mPosition);
	float spriteWidth = static_cast<float>( Graphics::TextureManager::Get()->GetTexture(mTexture)->GetWidth()) / mColumns;
	float spriteHeight = static_cast<float>(Graphics::TextureManager::Get()->GetTexture(mTexture)->GetHeight()) / mRows;
	int row = static_cast<int>(static_cast<float>(mFrame) / mColumns);
	int column = mFrame % mColumns;
	Math::Rect rect
	{
		 spriteWidth*column , spriteHeight*row,
		 spriteWidth*column + spriteWidth, spriteHeight *row + spriteHeight
	};
	Graphics::BatchRenderer::Get()->AddSprite(mTexture, rect, screenPos, 0.0f, mPivot, Graphics::Flip::None);
	if (mFrame == mFrameCount - 1)
		isActive = false;
}

void Particle::Start(const Math::Vector2 & pos)
{
	mFrame = 0;
	mFrameTimer = mDuration / mFrameCount;

	mPosition = pos; 
	isActive = true;
}
