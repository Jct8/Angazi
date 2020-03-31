#pragma once
#include "Projectile.h"
#include <Angazi/Inc/Angazi.h>

class ProjectileManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ProjectileManager& Get();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();

	void Fire(bool isPlayer,float speed, int damage, Angazi::Math::Vector2 startingPos, Angazi::Math::Vector2 targetPos);

private:
	std::deque<Projectile> mProjectiles;
	int mMaxProjectiles = 100;
};

