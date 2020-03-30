#include "Melee.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Player.h"

extern Player player;

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
	mSprite = X::LoadTexture(name);
	fscanf_s(file, "Particle:%s\n", name, maxsize);
	mParticleName = name;

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "Subtotal:%d\n", &subTotal);
		for (int j = 0; j < subTotal; j++)
		{
			fscanf_s(file, "%s\n", name, maxsize);
			mAnimationsMap[i].push_back(X::LoadTexture(name));
			fscanf_s(file, "%f,%f\n", &x, &y);
			mAnimationDamageMap[i].push_back({ x , y });
		}
	}
	fclose(file);

}

void MeleeWeapon::Unload()
{
}

void MeleeWeapon::Render(int mFrame, X::Math::Vector2 screenPos, bool isFacingLeft)
{
	//to fix bug - need to remove
	if (mFrame > mAnimationsMap[mCurrentAttack].size() - 1)
		mFrame = static_cast<int>(mAnimationsMap[mCurrentAttack].size()) - 1;

	if (isFacingLeft)
		X::DrawSprite(mAnimationsMap[mCurrentAttack][mFrame], screenPos);
	else
		X::DrawSprite(mAnimationsMap[mCurrentAttack][mFrame], screenPos, X::Pivot::Center, X::Flip::Horizontal);

	if (mFrame == mAnimationsMap[mCurrentAttack].size() - 1)
		mCurrentAttack = (mCurrentAttack + 1) % mAnimationsMap.size();
}

void MeleeWeapon::Attack(int mFrame, X::Math::Vector2 position, bool isFacingLeft, bool isPlayer)
{
	//to fix bug - need to remove
	if (mFrame > mAnimationsMap[mCurrentAttack].size() - 1)
		mFrame = static_cast<int>(mAnimationsMap[mCurrentAttack].size()) - 1;

	float width = mHitWidth;
	float height = mHitHeight;
	float damageOffsetX = mAnimationDamageMap[mCurrentAttack][mFrame].x;
	float damageOffsetY = mAnimationDamageMap[mCurrentAttack][mFrame].y;
	X::Math::Rect rect;
	X::Math::Vector2 particlePos = position;

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

	if (X::Math::Magnitude(mAnimationDamageMap[mCurrentAttack][mFrame]) == 0 || dealtDamage)
		return;

	if (isPlayer)
	{
		X::Math::Vector2 pos = EnemyManager::Get().CheckCollision(rect, mDamage,position);
		if (X::Math::Magnitude(pos) != 0)
		{
			ParticleManager::Get().StartParticle(pos,"BloodMelee",X::Pivot::Center);
			dealtDamage = true;
		}
		if (mParticleName != "None")
		{
			if (isFacingLeft)
				particlePos.x = position.x - ParticleManager::Get().GetParticleWidth(mParticleName);
			ParticleManager::Get().StartParticle(particlePos, mParticleName, X::Pivot::Left);
		}
	}
	else if (X::Math::Intersect( rect,player.GetBoundingBox() ))
	{
		player.TakeDamage(mDamage);
		ParticleManager::Get().StartParticle(player.GetPosition(),"BloodMelee", X::Pivot::Center);
		dealtDamage = true;
	}
}

int MeleeWeapon::GetFrameCount()
{
	return static_cast<int>(mAnimationDamageMap[mCurrentAttack].size());
}
