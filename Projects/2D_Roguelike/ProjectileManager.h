#pragma once
#include "Projectile.h"
#include <XEngine.h>

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

	void Fire(bool isPlayer,float speed, int damage, X::Math::Vector2 startingPos,X::Math::Vector2 targetPos);

private:
	std::vector<Projectile> mProjectiles;

};

