#include "Handgun.h"
#include "EnemyManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Handgun::Load()
{
	mSurvivorTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/survivor_handgun.png");
	mCrosshairTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/crosshair_handgun.png");
	mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/bullet_handgun.png");
	mDamage = 50;
	mAmmoCount = 10;
}

void Handgun::Unload()
{
	mSurvivorTextureId = 0;
	mCrosshairTextureId = 0;
	mTextureId = 0;
}

void Handgun::Render()
{
	Math::Vector2 pos;
	pos.x = 100.0f;
	pos.y = Graphics::GraphicsSystem::Get()->GetBackBufferHeight() - 100.0f;

	float time = MainApp().GetTime();
	if (isReloading && mReloadDelay < time)
	{
		mAmmoCount = 10;
		mReloadDelay = time + 1.0f;
		isReloading = false;
		mCrosshairTextureId = 0;
		mCrosshairTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/crosshair_shotgun.png");
	}

	for(int i = 0;  i< mAmmoCount; i++)
	{
		BatchRender::Get()->AddSprite(mTextureId,pos);
		pos.x+= 15.0f;
	}

}

void Handgun::Fire(const Math::Vector2 &position, const Math::Vector2 &direction)
{
	Math::LineSegment line(position + (direction *50.0f), position + (direction *1500.0f));
	SimpleDraw::AddScreenLine(position+ (direction *50.0f), position+ (direction *1500.0f) , Colors::Red);
	EnemyManager::Get().CheckCollision(line, mDamage);
	mAmmoCount--;
}

bool Handgun::CanFire() 
{
	float time = MainApp().GetTime();
	if (mFireDelay < time && mAmmoCount > 0 && !isReloading)
	{
		mFireDelay = time + 0.75f;
		return true;
	}
	return false;
}

void Handgun::Reload()
{
	//mAmmoCount = 10;
	isReloading = true;
	mReloadDelay = MainApp().GetTime() + 1.0f;
	mCrosshairTextureId = 0;
	mCrosshairTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/reload.png");
}
