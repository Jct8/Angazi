#include "EnemyManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "ParticleManager.h"

extern Player player;
using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	const int maxsize = 100;
	Animation2D ConvertToEnum(std::string name)
	{
		if (name == "Idle")     return Idle;
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
		Animation2D anim = ConvertToEnum(animS);
		mAnimationsDuration[anim] = duration;

		for (int j = 0; j < subTotal; j++)
		{
			fscanf_s(file, "%s\n", name, maxsize);
			//Angazi::Graphics::Texture tex;
			//tex.Initialize("../../Assets/Images/Rougelike/" + std::string(name));
			mAnimations[anim].emplace_back();
			mAnimations[anim].back() = TextureManager::Get()->Load("../../Assets/Images/Rougelike/" + std::string(name));
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
	for (auto& animationType : mAnimations)
		for(auto& animation : animationType.second)
			animation = 0;
	mWeapon->Unload();
	mWeapon.reset();
}

void Enemy::Update(float deltaTime)
{
	if (!isAlive)
	{
		if (mDeathDelay < MainApp().GetTime())
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

	Math::Vector2 displacement;
	displacement.y = mSpeed * deltaTime;

	//Movement
	Math::LineSegment visionLine{ mPosition.x - mDetectRange, mPosition.y,mPosition.x + mDetectRange, mPosition.y };
	Math::LineSegment line = Camera2D::Get().ConvertToScreenPosition(visionLine);
	SimpleDraw::AddScreenLine(line.from, line.to, Colors::AliceBlue);

	Math::Rect playerBox = player.GetBoundingBox();
	if (visionLine.to.y > playerBox.top && visionLine.to.y < playerBox.bottom
		&& (visionLine.from.x < playerBox.right && visionLine.to.x > playerBox.left))
	{
		mHeading = Math::Normalize(player.GetPosition() - mPosition);
		displacement.x += mHeading.x * mSpeed *deltaTime;
		if (mHeading.x < 0.0f)
			isFacingLeft = true;
		else if (mHeading.x > 0.0f)
			isFacingLeft = false;
	}
	if (Math::Abs(player.GetPosition().x - mPosition.x) < mStoppingRange
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
		Math::LineSegment BottomEdge{
			currentBox.left,
			currentBox.bottom + displacement.y,
			currentBox.right,
			currentBox.bottom + displacement.y,
		};
		if (TileMap::Get().CheckCollision(BottomEdge))
		{
			displacement.y = 0.0f;
		}
	}
	if (displacement.x < 0.0f)
	{
		Math::LineSegment leftEdge{
			currentBox.left + displacement.x,	currentBox.top,
			currentBox.left + displacement.x,	currentBox.bottom,
		};
		Math::LineSegment groundEdge{
			currentBox.left + displacement.x,	currentBox.bottom - 16.0f,
			currentBox.left + displacement.x,	currentBox.bottom + 16.0f,
		};
		if (TileMap::Get().CheckCollision(leftEdge) || !TileMap::Get().CheckCollision(groundEdge))
		{
			displacement.x = 0.0f;
		}
	}
	if (displacement.x > 0.0f)
	{
		Math::LineSegment rightEdge{
			currentBox.right + displacement.x,
			currentBox.top,
			currentBox.right + displacement.x,
			currentBox.bottom,
		};
		Math::LineSegment groundEdge{
			currentBox.right + displacement.x,	currentBox.bottom - 16.0f,
			currentBox.right + displacement.x,	currentBox.bottom + 16.0f,
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
	Math::Vector2 screenPos = Camera2D::Get().ConvertToScreenPosition(mPosition);

	if (mCurrentAnimation == Attacking)
		mWeapon->Render(mFrame, screenPos, isFacingLeft);
	else if (isFacingLeft)
		BatchRenderer::Get()->AddSprite(mAnimations[mCurrentAnimation][mFrame], screenPos);
	else
		BatchRenderer::Get()->AddSprite(mAnimations[mCurrentAnimation][mFrame], screenPos, 0.0f , Pivot::Center, Flip::Horizontal);

	if (mFrame == mFrameCount - 1 && isAlive)
	{
		ChangeAnimation(Idle);
		mWeapon->SetDealtDamage(false);
	}

	if (mShowDebug)
	{
		Math::Vector2 screenPos2 = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ position.x ,position.y });
		float offset = 32.0f * 0.5f;
		SimpleDraw::AddScreenCircle({ screenPos2.x, screenPos2.y - offset }, 10.0f, Colors::AliceBlue);

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
		ParticleManager::Get().StartParticle(mPosition, "BloodMelee", Pivot::Center);
		ParticleManager::Get().StartParticle(mPosition, "particleSheet", Pivot::Center);
		mHealth = 0;
		isAlive = false;
		EnemyManager::Get().IncreaseDeathCount();
		mDeathDelay = MainApp().GetTime() + 0.5f;
		ChangeAnimation(Death);
	}
}

void Enemy::ChangeAnimation(Animation2D animation, bool overrideChange)
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

Math::Rect Enemy::GetBoundingBox()
{
	return
	{
		mPosition.x - mWidth,
		mPosition.y - mHeight,
		mPosition.x + mWidth,
		mPosition.y + mHeight
	};
}
