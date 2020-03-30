#pragma once
#include "Enemy.h"
#include "Survivor.h"
#include <Angazi/Inc/Angazi.h>

class EnemyManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyManager& Get();

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void CheckCollision(Angazi::Math::LineSegment line,int damage);
	void CheckPlayerCollision();

	void AlertEnemies();

	void IncreaseDeathCount() { mTotalDeaths++; };

	int GetZombieScore() const { return mTotalDeaths; }

private:
	std::vector<Enemy*> mEnemies;
	float mNextSpawnTime = 0.0f;

	bool mAllowSpawning;
	int mTotalDeaths = 1;
	int mSpawnLevel = 1;

	float randomFloat1 = 1.0f;
	float randomFloat2 = 3.0f;
};

