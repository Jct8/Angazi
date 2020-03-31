#include "Projectile.h"
#include "Camera2D.h"
#include "TileMap.h"
#include "EnemyManager.h"
#include "Player.h"

extern Player player;
using namespace Angazi;
using namespace Angazi::Graphics;

void Projectile::Load(std::string fileName)
{
	mTexture.Initialize("../../Assets/Images/Rougelike/" + fileName);
	mBloodTexture.Initialize("../../Assets/Images/Rougelike/blood.png");
	isActive = false;
}

void Projectile::Unload()
{
	mTexture.Terminate();
	mBloodTexture.Terminate();
}

void Projectile::Update(float deltaTime)
{
	if (isPlayingAnimation)
	{
		mFrameTimer -= deltaTime;
		if (mFrameTimer <= 0.0f)
		{
			mFrameTimer = mDuration/ mFrameCount;
			mFrame = (mFrame + 1) % mFrameCount;
		}
		return;
	}

	mPosition += mDirection * deltaTime * mSpeed;
	if (Math::Distance(mPosition,mStartingPosition) >= mProjectileFallDist)
	{
		mPosition.y += deltaTime * mGravity;
	}

	//World Collision
	Math::LineSegment line{
			mPosition.x, mPosition.y,
			mPosition.x, mPosition.y,
	};
	if (TileMap::Get().CheckCollision(line))
		isActive = false;

	//Enemy Collisions
	if (mIsPlayer && EnemyManager::Get().CheckProjectileCollision(mPosition, mDamage))
	{
		isPlayingAnimation = true;
		return;
	}

	//PlayerCollisions
	if (!mIsPlayer && Math::PointInRect(mPosition, player.GetBoundingBox()))
	{
		player.TakeDamage(mDamage);
		isPlayingAnimation = true;
		return;
	}
}

void Projectile::Render()
{
	if (isPlayingAnimation)
	{
		float spriteWidth = static_cast<float>(mBloodTexture.GetWidth() / 4.0f);
		float spriteHeight = static_cast<float>(mBloodTexture.GetHeight() / 4.0f);
		int row = static_cast<int>(static_cast<float>(mFrame) / 4.0f);
		int column = mFrame % 4;
		Math::Rect rect
		{
			 spriteWidth*column , spriteHeight*row,
			 spriteWidth*column + spriteWidth, spriteHeight *row + spriteHeight
		};
		auto screenPos = Camera2D::Get().ConvertToScreenPosition(mPosition);
		SpriteRenderer::Get()->Draw(mBloodTexture, rect, screenPos, 0.0f , Pivot::Center, Flip::None);
		if (mFrame == mFrameCount - 1)
			isActive = false;
		return;
	}
	auto screenPos = Camera2D::Get().ConvertToScreenPosition(mPosition);
	SpriteRenderer::Get()->Draw(mTexture, screenPos);
}

void Projectile::Fire(bool isPlayer, float speed,int damage, Math::Vector2 targetPos, Math::Vector2 startingPos)
{
	mDuration = 0.2f;
	mFrame = 0;
	mFrameCount = 16;
	mFrameTimer = mDuration / mFrameCount;

	isPlayingAnimation = false;
	mIsPlayer = isPlayer;
	mDamage = damage;
	isActive = true;
	mPosition = startingPos;
	mStartingPosition = startingPos;
	mDirection = Math::Normalize(targetPos - mPosition);
	mSpeed = speed;
}