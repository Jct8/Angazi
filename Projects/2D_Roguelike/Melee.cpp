#include "Melee.h"
#include "Camera2D.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Player.h"

extern Player player;
using namespace Angazi;
using namespace Angazi::Graphics;

void MeleeWeapon::Load(std::filesystem::path fileName)
{
	const int maxsize = 100;

	char name[maxsize];
	int total = 0;
	int subTotal = 0;
	float x, y;
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	fscanf_s(file, "TotalAnimations:%d\n", &total);
	fscanf_s(file, "HitSize:%f,%f\n", &mHitWidth, &mHitHeight);
	fscanf_s(file, "Damage:%d\n", &mDamage);
	fscanf_s(file, "%s\n", name, maxsize);
	mSprite = TextureManager::Get()->Load("../../Assets/Images/Rougelike/" + std::string(name));
	fscanf_s(file, "Particle:%s\n", name, maxsize);
	mParticleName = name;

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "Subtotal:%d\n", &subTotal);
		for (int j = 0; j < subTotal; j++)
		{
			fscanf_s(file, "%s\n", name, maxsize);
			mAnimationsMap[i].emplace_back();
			mAnimationsMap[i][j] = TextureManager::Get()->Load("../../Assets/Images/Rougelike/" + std::string(name));
			fscanf_s(file, "%f,%f\n", &x, &y);
			mAnimationDamageMap[i].push_back({ x , y });
		}
	}
	fclose(file);

}

void MeleeWeapon::Unload()
{
	for (auto& animationType : mAnimationsMap)
		for (auto& animation : animationType.second)
			animation = 0;

	mSprite = 0;
	for (auto& animations : mAnimations)
		animations = 0;
}

void MeleeWeapon::Render(int mFrame, Math::Vector2 screenPos, bool isFacingLeft)
{
	//to fix bug - need to remove
	if (static_cast<size_t>(mFrame) > mAnimationsMap[mCurrentAttack].size() - 1)
		mFrame = static_cast<int>(mAnimationsMap[mCurrentAttack].size()) - 1;

	if (isFacingLeft)
		BatchRenderer::Get()->AddSprite(mAnimationsMap[mCurrentAttack][mFrame], screenPos);
	else
		BatchRenderer::Get()->AddSprite(mAnimationsMap[mCurrentAttack][mFrame], screenPos, 0.0f, Pivot::Center, Flip::Horizontal);

	if (mFrame == mAnimationsMap[mCurrentAttack].size() - 1)
		mCurrentAttack = (mCurrentAttack + 1) % mAnimationsMap.size();
}

void MeleeWeapon::Attack(int mFrame, Math::Vector2 position, bool isFacingLeft, bool isPlayer)
{
	//to fix bug - need to remove
	if (static_cast<size_t>(mFrame) > mAnimationsMap[mCurrentAttack].size() - 1)
		mFrame = static_cast<int>(mAnimationsMap[mCurrentAttack].size()) - 1;

	float width = mHitWidth;
	float height = mHitHeight;
	float damageOffsetX = mAnimationDamageMap[mCurrentAttack][mFrame].x;
	float damageOffsetY = mAnimationDamageMap[mCurrentAttack][mFrame].y;
	Math::Rect rect;
	Math::Vector2 particlePos = position;

	if (isFacingLeft)
	{
		damageOffsetX = -damageOffsetX;
		width = -width;
		height = -height;
		rect.right = position.x + damageOffsetX;
		rect.bottom = position.y + damageOffsetY - height;
		rect.left = position.x + damageOffsetX + width;
		rect.top = position.y + damageOffsetY + height;

	}
	else
	{
		rect.left = position.x + damageOffsetX;
		rect.top = position.y + damageOffsetY - height;
		rect.right = position.x + damageOffsetX + width;
		rect.bottom = position.y + damageOffsetY + height;
	}

	//X::Math::Rect rect2 = Camera::Get().ConvertToScreenPosition(rect);
	//if (X::Math::Magnitude(mAnimationDamageMap[mCurrentAttack][mFrame]) != 0)
	//	X::DrawScreenRect(rect2, X::Colors::Red);

	if (Math::Magnitude(mAnimationDamageMap[mCurrentAttack][mFrame]) == 0 || dealtDamage)
		return;

	if (isPlayer)
	{
		Math::Vector2 pos = EnemyManager::Get().CheckCollision(rect, mDamage,position);
		if (Math::Magnitude(pos) != 0)
		{
			ParticleManager::Get().StartParticle(pos,"BloodMelee", Pivot::Center);
			dealtDamage = true;
		}
		if (mParticleName != "None")
		{
			if (isFacingLeft)
				particlePos.x = position.x - ParticleManager::Get().GetParticleWidth(mParticleName);
			ParticleManager::Get().StartParticle(particlePos, mParticleName, Pivot::Left);
		}
	}
	else if (Math::Intersect( rect ,player.GetBoundingBox() ))
	{
		player.TakeDamage(mDamage);
		ParticleManager::Get().StartParticle(player.GetPosition(),"BloodMelee", Pivot::Center);
		dealtDamage = true;
	}
}

int MeleeWeapon::GetFrameCount()
{
	return static_cast<int>(mAnimationDamageMap[mCurrentAttack].size());
}
