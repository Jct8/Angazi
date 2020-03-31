#pragma once
#include "Enemy.h"
#include <Angazi/Inc/Angazi.h>

class EnemyManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyManager& Get();

	void Load(Angazi::AI::AIWorld& world);
	void Unload();

	void Update(float deltaTime);
	void Render();

	Angazi::Math::Vector2 CheckCollision(Angazi::Math::Rect rect,int damage, Angazi::Math::Vector2 pos);
	bool CheckProjectileCollision(Angazi::Math::Vector2 point, int damage);

	void IncreaseDeathCount() { mTotalDeaths++; };
	void DebugUI();

private:
	std::deque<Enemy> mEnemies;
	int mTotalDeaths = 0;

	bool mShowDebug = true;
};

