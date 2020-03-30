#include "Projectile.h"
#include "Camera.h"
#include "TileMap.h"
#include "EnemyManager.h"
#include "Player.h"

extern Player player;

void Projectile::Load(std::string fileName)
{
	mTexture = X::LoadTexture(fileName.c_str());
	mBloodTexture = X::LoadTexture("blood.png");
	isActive = false;
}

void Projectile::Unload()
{
	mTexture = 0;
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
	if (X::Math::Distance(mPosition,mStartingPosition) >= mProjectileFallDist)
	{
		mPosition.y += deltaTime * mGravity;
	}

	//World Collision
	X::Math::LineSegment line{
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
	if (!mIsPlayer && X::Math::PointInRect(mPosition, player.GetBoundingBox()))
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
		float spriteWidth = static_cast<float>(X::GetSpriteWidth(mBloodTexture)) / 4.0f;
		float spriteHeight = static_cast<float>(X::GetSpriteHeight(mBloodTexture)) / 4.0f;
		int row = static_cast<int>(static_cast<float>(mFrame) / 4.0f);
		int column = mFrame % 4;
		X::Math::Rect rect
		{
			 spriteWidth*column , spriteHeight*row,
			 spriteWidth*column + spriteWidth, spriteHeight *row + spriteHeight
		};
		auto screenPos = Camera::Get().ConvertToScreenPosition(mPosition);
		X::DrawSprite(mBloodTexture, rect, screenPos/*, X::Pivot::Center, X::Flip::None*/);
		if (mFrame == mFrameCount - 1)
			isActive = false;
		return;
	}
	auto screenPos = Camera::Get().ConvertToScreenPosition(mPosition);
	X::DrawSprite(mTexture, screenPos);
}

void Projectile::Fire(bool isPlayer, float speed,int damage, X::Math::Vector2 targetPos, X::Math::Vector2 startingPos)
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
	mDirection = X::Math::Normalize(targetPos - mPosition);
	mSpeed = speed;
}