#include "Shotgun.h"
#include "EnemyManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Shotgun::Load()
{
	mSurvivorTextureId = TextureManager::Get()->Load(Angazi::Core::FilePath::GetAssetFilePath() + "Images/XEngine/survivor_shotgun.png");
	mCrosshairTextureId = TextureManager::Get()->Load(Angazi::Core::FilePath::GetAssetFilePath() + "Images/XEngine/crosshair_shotgun.png");
	mTextureId = TextureManager::Get()->Load(Angazi::Core::FilePath::GetAssetFilePath() + "Images/XEngine/bullet_shotgun.png");
	mDamage = 20;
	mAmmoCount = 15;
}

void Shotgun::Unload()
{
	mSurvivorTextureId = 0;
	mCrosshairTextureId = 0;
	mTextureId =0;
}

void Shotgun::Render()
{
	Math::Vector2 pos;
	pos.x = 100.0f;
	pos.y = GraphicsSystem::Get()->GetBackBufferHeight()- 100.0f;

	float time = MainApp().GetTime();
	if (isReloading && mReloadDelay < time)
	{
		mAmmoCount = 15;
		mReloadDelay = time + 1.0f;
		isReloading = false;
		mCrosshairTextureId = 0;
		mCrosshairTextureId = TextureManager::Get()->Load(Angazi::Core::FilePath::GetAssetFilePath() + "Images/XEngine/crosshair_shotgun.png");
	}

	for (int i = 0; i < mAmmoCount; i++)
	{
		BatchRenderer::Get()->AddSprite(mTextureId, pos);
		pos.x += 15.0f;
	}
}

void Shotgun::Fire(const Math::Vector2 &position, const Math::Vector2 &direction)
{
	SimpleDraw::AddScreenLine(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, -0.25f), Colors::Red);
	SimpleDraw::AddScreenLine(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, -0.12f), Colors::Red);
	SimpleDraw::AddScreenLine(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.0f), Colors::Red);
	SimpleDraw::AddScreenLine(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.12f), Colors::Red);
	SimpleDraw::AddScreenLine(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.25f), Colors::Red);

	Math::LineSegment line1(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, -0.25f));
	EnemyManager::Get().CheckCollision(line1, mDamage);
	Math::LineSegment line2(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, -0.12f));
	EnemyManager::Get().CheckCollision(line2, mDamage);
	Math::LineSegment line3(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.0f));
	EnemyManager::Get().CheckCollision(line3, mDamage);
	Math::LineSegment line4(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.12f));
	EnemyManager::Get().CheckCollision(line4, mDamage);
	Math::LineSegment line5(position + Math::Rotate(direction *75.0f, 0.0f), position + Math::Rotate(direction * 500.0f, 0.25f));
	EnemyManager::Get().CheckCollision(line5, mDamage);

	mAmmoCount -= 5;
}

bool Shotgun::CanFire()
{
	float time = MainApp().GetTime();
	if (mFireDelay < time && mAmmoCount > 0 && !isReloading)
	{
		mFireDelay = time + 1.0f;
		return true;
	}
	return false;
}

void Shotgun::Reload()
{
	//mAmmoCount = 15;
	isReloading = true;
	mReloadDelay = MainApp().GetTime() + 1.0f;
	mCrosshairTextureId = 0;
	mCrosshairTextureId = TextureManager::Get()->Load(Angazi::Core::FilePath::GetAssetFilePath() + "Images/XEngine/reload.png");
}
