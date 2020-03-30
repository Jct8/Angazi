#include "Projectile.h"
#include "ProjectileManager.h"

namespace
{
	ProjectileManager* sInstance = nullptr;
}

void ProjectileManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "ProjectileManager already initialized");
	sInstance = new ProjectileManager();
}

void ProjectileManager::StaticTerminate()
{
	delete sInstance;
	sInstance = nullptr;
}

ProjectileManager & ProjectileManager::Get()
{
	XASSERT(sInstance != nullptr, "No ProjectileManager created!");
	return *sInstance;
}

void ProjectileManager::Load()
{
	for (int i = 0; i < 100; i++)
	{
		mProjectiles.push_back(Projectile());
		mProjectiles[i].Load("Projectile.png");
	}
}

void ProjectileManager::Unload()
{
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

void ProjectileManager::Fire(bool isPlayer,float speed,int damage, X::Math::Vector2 startingPos ,X::Math::Vector2 targetPos)
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
