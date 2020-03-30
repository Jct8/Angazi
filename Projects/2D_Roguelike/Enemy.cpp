#include "EnemyManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "ParticleManager.h"

extern Player player;

namespace
{
	const int maxsize = 100;
	Animation ConvertToEnum(std::string name)
	{
		if (name == "Idle")return Idle;
		if (name == "Attacking")return Attacking;
		if (name == "Running")  return Running;
		if (name == "Jumping")  return Jumping;
		if (name == "Death")    return Death;
		return Idle;
	}
}

Enemy::Enemy(AI::AIWorld & world, uint32_t typeId)
	:Agent(world, typeId)
{
}

void Enemy::Load(std::filesystem::path fileName, bool facingLeft)
{
	isFacingLeft = facingLeft;
	char animString[maxsize];
	char name[maxsize];
	int total = 0;
	int subTotal = 0;
	float duration;

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	fscanf_s(file, "Total:%d\n", &total);
	fscanf_s(file, "Width:%f\n", &mWidth);
	fscanf_s(file, "Height:%f\n", &mHeight);
	fscanf_s(file, "FollowRange:%f\n", &mDetectRange);
	fscanf_s(file, "StoppingRange:%f\n", &mStoppingRange);
	fscanf_s(file, "%s\n", name, maxsize);
	weaponType = name;
	fscanf_s(file, "%s\n", name, maxsize);
	std::string weaponFileName(name);

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%s %d %f\n", animString, maxsize, &subTotal, &duration);
		std::string animS(animString);
		Animation anim = ConvertToEnum(animS);
		mAnimationsDuration[anim] = duration;

		for (int j = 0; j < subTotal; j++)
		{
			fscanf_s(file, "%s\n", name, maxsize);
			mAnimations[anim].push_back(X::LoadTexture(name));
		}
	}
	fclose(file);

	mDuration = 0.5f;
	mFrame = 0;
	mFrameCount = static_cast<int>(mAnimations[Idle].size());
	mFrameTimer = mAnimationsDuration[Idle] / mFrameCount;

	mCurrentAnimation = Idle;

	if (weaponType == "Melee")
		mWeapon = std::make_unique<MeleeWeapon>();
	else if (weaponType == "Ranged")
		mWeapon = std::make_unique<RangedWeapon>();
	//if (!mWeapon->IsMelee())
	//	auto ranged = static_cast<RangedWeapon*>(mWeapon.get());
	mWeapon->Load(weaponFileName.c_str());
}

void Enemy::Unload()
{

}

void Enemy::Update(float deltaTime)
{
	if (!isAlive)
	{
		if (mDeathDelay < X::GetTime())
		{
			return;
		}
		mFrameTimer -= deltaTime;
		if (mFrameTimer <= 0.0f)
		{
			mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
			mFrame = (mFrame + 1) % mFrameCount;
		}
		return;
	}

	X::Math::Vector2 displacement;
	displacement.y = mSpeed * deltaTime;

	//Movement
	X::Math::LineSegment visionLine{ mPosition.x - mDetectRange, mPosition.y,mPosition.x + mDetectRange, mPosition.y };
	X::Math::LineSegment line = Camera::Get().ConvertToScreenPosition(visionLine);
	X::DrawScreenLine(line.from, line.to, X::Colors::AliceBlue);

	X::Math::Rect playerBox = player.GetBoundingBox();
	if (visionLine.to.y > playerBox.top && visionLine.to.y < playerBox.bottom
		&& (visionLine.from.x < playerBox.right && visionLine.to.x > playerBox.left))
	{
		mHeading = X::Math::Normalize(player.GetPosition() - mPosition);
		displacement.x += mHeading.x * mSpeed *deltaTime;
		if (mHeading.x < 0.0f)
			isFacingLeft = true;
		else if (mHeading.x > 0.0f)
			isFacingLeft = false;
	}
	if (X::Math::Abs(player.GetPosition().x - mPosition.x) < mStoppingRange
		&& visionLine.to.y > playerBox.top && visionLine.to.y < playerBox.bottom)
	{
		displacement.x = 0.0f;
		Attack();
		ChangeAnimation(Attacking);
		/*if (mAttackDelay < X::GetTime())
		{
			mAttackDelay = X::GetTime() + 3.0f;
		}*/

	}

	//Collisions
	auto currentBox = GetBoundingBox();
	if (displacement.y > 0.0f)
	{
		X::Math::LineSegment BottomEdge{
			currentBox.min.x,
			currentBox.max.y + displacement.y,
			currentBox.max.x,
			currentBox.max.y + displacement.y,
		};
		if (TileMap::Get().CheckCollision(BottomEdge))
		{
			displacement.y = 0.0f;
		}
	}
	if (displacement.x < 0.0f)
	{
		X::Math::LineSegment leftEdge{
			currentBox.min.x + displacement.x,	currentBox.min.y,
			currentBox.min.x + displacement.x,	currentBox.max.y,
		};
		X::Math::LineSegment groundEdge{
			currentBox.min.x + displacement.x,	currentBox.max.y - 16.0f,
			currentBox.min.x + displacement.x,	currentBox.max.y + 16.0f,
		};
		if (TileMap::Get().CheckCollision(leftEdge) || !TileMap::Get().CheckCollision(groundEdge))
		{
			displacement.x = 0.0f;
		}
	}
	if (displacement.x > 0.0f)
	{
		X::Math::LineSegment rightEdge{
			currentBox.max.x + displacement.x,
			currentBox.min.y,
			currentBox.max.x + displacement.x,
			currentBox.max.y,
		};
		X::Math::LineSegment groundEdge{
			currentBox.max.x + displacement.x,	currentBox.max.y - 16.0f,
			currentBox.max.x + displacement.x,	currentBox.max.y + 16.0f,
		};
		if (TileMap::Get().CheckCollision(rightEdge) || !TileMap::Get().CheckCollision(groundEdge))
		{
			displacement.x = 0.0f;
		}
	}

	if (displacement.x != 0.0f)
	{
		ChangeAnimation(Running);
	}

	//Animation
	mFrameTimer -= deltaTime;
	if (mFrameTimer <= 0.0f)
	{
		mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
		mFrame = (mFrame + 1) % mFrameCount;
	}

	mPosition += displacement;
	position += displacement;
}

void Enemy::Render()
{
	X::Math::Vector2 screenPos = Camera::Get().ConvertToScreenPosition(mPosition);

	if (mCurrentAnimation == Attacking)
		mWeapon->Render(mFrame, screenPos, isFacingLeft);
	else if (isFacingLeft)
		X::DrawSprite(mAnimations[mCurrentAnimation][mFrame], screenPos);
	else
		X::DrawSprite(mAnimations[mCurrentAnimation][mFrame], screenPos, X::Pivot::Center, X::Flip::Horizontal);

	if (mFrame == mFrameCount - 1 && isAlive)
	{
		ChangeAnimation(Idle);
		mWeapon->SetDealtDamage(false);
	}

	if (mShowDebug)
	{
		X::Math::Vector2 screenPos2 = Camera::Get().ConvertToScreenPosition(X::Math::Vector2{ position.x ,position.y });
		float offset = 32.0f * 0.5f;
		X::DrawScreenCircle({ screenPos2.x, screenPos2.y - offset }, 10.0f, X::Colors::AliceBlue);

		//Hitbox
		//X::Math::Rect rect = Camera::Get().ConvertToScreenPosition(GetBoundingBox());
		//X::DrawScreenRect(rect, X::Colors::Green);
	}
}

void Enemy::TakeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth <= 0 && isAlive)
	{
		ParticleManager::Get().StartParticle(mPosition, "BloodMelee", X::Pivot::Center);
		ParticleManager::Get().StartParticle(mPosition, "particleSheet", X::Pivot::Center);
		mHealth = 0;
		isAlive = false;
		EnemyManager::Get().IncreaseDeathCount();
		mDeathDelay = X::GetTime() + 0.5f;
		ChangeAnimation(Death);
	}
}

void Enemy::ChangeAnimation(Animation animation, bool overrideChange)
{
	if (animation == mCurrentAnimation)
		return;
	mCurrentAnimation = animation;
	if (mCurrentAnimation == Attacking)
		mFrameCount = mWeapon->GetFrameCount();
	else
		mFrameCount = static_cast<int>(mAnimations[animation].size());
	mFrame = 0;
	mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
}

void Enemy::Attack()
{
	mWeapon->Attack(mFrame, mPosition, isFacingLeft, false);
}

X::Math::Rect Enemy::GetBoundingBox()
{
	return
	{
		mPosition.x - mWidth,
		mPosition.y - mHeight,
		mPosition.x + mWidth,
		mPosition.y + mHeight
	};
}
