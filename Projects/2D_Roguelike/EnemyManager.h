#pragma once
#include "Enemy.h"
#include <deque>
#include <XEngine.h>

class EnemyManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyManager& Get();

	void Load(AI::AIWorld& world);
	void Unload();

	void Update(float deltaTime);
	void Render();

	X::Math::Vector2 CheckCollision(X::Math::Rect rect,int damage, X::Math::Vector2 pos);
	bool CheckProjectileCollision(X::Math::Vector2 point, int damage);

	void IncreaseDeathCount() { mTotalDeaths++; };
	void DebugUI();

private:
	std::deque<Enemy> mEnemies;
	int mTotalDeaths = 0;

	bool mShowDebug = true;
};

