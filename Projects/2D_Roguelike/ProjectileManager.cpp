#include "Projectile.h"
#include "ProjectileManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<ProjectileManager> sInstance = nullptr;
}

void ProjectileManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "ProjectileManager already initialized");
	sInstance = std::make_unique<ProjectileManager>();
}

void ProjectileManager::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

ProjectileManager & ProjectileManager::Get()
{
	ASSERT(sInstance != nullptr, "No ProjectileManager created!");
	return *sInstance;
}

void ProjectileManager::Load()
{
	for (int i = 0; i < mMaxProjectiles; i++)
	{
		mProjectiles.emplace_back();
		mProjectiles[i].Load("Projectile.png");
	}
}

void ProjectileManager::Unload()
{
	for (int i = 0; i < mMaxProjectiles; i++)
	{
		mProjectiles[i].Unload();
	}
}

void ProjectileManager::Update(float deltaTime)
{
	for (int i = 0; i < mProjectiles.size(); i++)
	{
		if (mProjectiles[i].IsActive())
		{
			mProjectiles[i].Update(deltaTime);
		}
	}
}

void ProjectileManager::Render()
{
	for (int i = 0; i < mProjectiles.size(); i++)
	{
		if (mProjectiles[i].IsActive())
		{
			mProjectiles[i].Render();
		}
	}
}

void ProjectileManager::Fire(bool isPlayer,float speed,int damage, Math::Vector2 startingPos , Math::Vector2 targetPos)
{
	for (int i = 0; i < mProjectiles.size(); i++)
	{
		if (!mProjectiles[i].IsActive())
		{
			mProjectiles[i].Fire(isPlayer,speed,damage, targetPos,startingPos);
			break;
		}
	}
}
