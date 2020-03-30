#include "ProjectileManager.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Ranged.h"

extern Player player;

void RangedWeapon::Load(std::filesystem::path fileName)
{
	const int maxsize = 100;

	char name[maxsize];
	int total = 0;
	float x = 0.0f, y = 0.0f;
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	fscanf_s(file, "Total:%d\n", &total);
	fscanf_s(file, "Offset:%f,%f\n", &mOffset.x, &mOffset.y);
	fscanf_s(file, "Speed:%f\n", &mSpeed);
	fscanf_s(file, "Damage:%d\n", &mDamage);
	fscanf_s(file, "Projectiles:%d\n", &mTotalProjectiles);
	fscanf_s(file, "%s\n", name, maxsize);
	mSprite = X::LoadTexture(name);

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%s\n", name, maxsize);
		mAnimations.push_back(X::LoadTexture(name));
	}
	fclose(file);
	mAttackDelay = 1.0f;
}

void RangedWeapon::Unload()
{
}

void RangedWeapon::Render(int mFrame, X::Math::Vector2 screenPos, bool isFacingLeft)
{
	if (isFacingLeft)
		X::DrawSprite(mAnimations[mFrame], screenPos);
	else
		X::DrawSprite(mAnimations[mFrame], screenPos, X::Pivot::Center, X::Flip::Horizontal);
}

void RangedWeapon::Attack(int mFrame, X::Math::Vector2 screenPos, bool isFacingLeft, bool isPlayer)
{
	if (dealtDamage)
		return;
	X::Math::Vector2 offset = mOffset;
	X::Math::Vector2 targetPos;
	if (isFacingLeft)
	{
		targetPos = screenPos + offset + X::Math::Vector2{ -1.0f, 0 };
	}
	else
		targetPos = screenPos + offset + X::Math::Vector2{ 1.0f, 0 };

	if (mFrame != 6 && isPlayer)
	{
		return;
	}
	auto firePos = screenPos + offset;
	for (int i = 0; i < mTotalProjectiles; i++)
	{
		firePos.y += 10.0f;
		targetPos.y += 10.0f;
		ProjectileManager::Get().Fire(isPlayer, mSpeed, mDamage, firePos, targetPos);
	}
	dealtDamage = true;
}
