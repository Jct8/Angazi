#include "ProjectileManager.h"
#include "Camera2D.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Ranged.h"

extern Player player;
using namespace Angazi;
using namespace Angazi::Graphics;

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
	mSprite.Initialize("../../Assets/Images/Rougelike/" + std::string(name));

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%s\n", name, maxsize);
		//Angazi::Graphics::Texture tex;
		//tex.Initialize(name);
		mAnimations.emplace_back();
		mAnimations.back().Initialize("../../Assets/Images/Rougelike/" + std::string(name));
	}
	fclose(file);
	mAttackDelay = 1.0f;
}

void RangedWeapon::Unload()
{
	mSprite.Terminate();
	for (auto& animations : mAnimations)
		animations.Terminate();
}

void RangedWeapon::Render(int mFrame, Math::Vector2 screenPos, bool isFacingLeft)
{
	if (isFacingLeft)
		SpriteRenderer::Get()->Draw(mAnimations[mFrame], screenPos);
	else
		SpriteRenderer::Get()->Draw(mAnimations[mFrame], screenPos, 0.0f , Pivot::Center, Flip::Horizontal);
}

void RangedWeapon::Attack(int mFrame, Math::Vector2 screenPos, bool isFacingLeft, bool isPlayer)
{
	if (dealtDamage)
		return;
	Math::Vector2 offset = mOffset;
	Math::Vector2 targetPos;
	if (isFacingLeft)
	{
		targetPos = screenPos + offset + Math::Vector2{ -1.0f, 0 };
	}
	else
		targetPos = screenPos + offset + Math::Vector2{ 1.0f, 0 };

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
