#include "EnemyManager.h"

extern Survivor survivor;

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	EnemyManager* sInstance = nullptr;
}

void EnemyManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "EnemyManger already initialized");
	sInstance = new EnemyManager();
}

void EnemyManager::StaticTerminate()
{
	sInstance->Unload();
	delete sInstance;
	sInstance = nullptr;
}

EnemyManager & EnemyManager::Get()
{
	ASSERT(sInstance != nullptr, "No EnemyManger created!");
	return *sInstance;
}

void EnemyManager::Load()
{
}

void EnemyManager::Unload()
{
	mNextSpawnTime = 0.0f;

	mAllowSpawning;
	mTotalDeaths = 1;
	mSpawnLevel = 1;

	randomFloat1 = 1.0f;
	randomFloat2 = 3.0f;

	for (auto enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
		enemy = nullptr;
	}

	mEnemies.clear();
}

void EnemyManager::Update(float deltaTime)
{
	if (mNextSpawnTime < MainApp().GetTime())
	{
		Enemy* enemy = new Enemy();
		enemy->Load();
		float x = Angazi::Math::RandomFloat(0.0f, static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth())  );
		float y = Angazi::Math::RandomFloat(0.0f, static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight()) );

		enemy->SetPosition({50.0f,50.0f});
		mEnemies.push_back(enemy);

		if (mTotalDeaths / 5  == mSpawnLevel)
		{
			mSpawnLevel++;
			if(randomFloat1 - 0.1f >= 0.1f )
				randomFloat1 -= 0.1f;
			if (randomFloat2 - 0.2f >= 0.3f)
				randomFloat2 -=0.2f;
		}
		float spawnTime = 3.0f;//X::RandomFloat(randomFloat1, randomFloat2);
		mNextSpawnTime = MainApp().GetTime() +  spawnTime;
		
	}

	for (auto enemy : mEnemies)
		enemy->Update(deltaTime);
	
}

void EnemyManager::Render()
{
	for (auto enemy : mEnemies)
		enemy->Render();
	std::string str = "Spawn level :" + std::to_string(mSpawnLevel);
	//X::DrawScreenText(str.c_str(), 50.0f, 20.0f, 30.0f, X::Colors::Blue);
	std::string str2 = "Zombies Killed :" + std::to_string(mTotalDeaths);
	//X::DrawScreenText(str2.c_str(), 50.0f, 50.0f, 30.0f, X::Colors::Blue);
}

void EnemyManager::CheckCollision(Angazi::Math::LineSegment line, int damage)
{
	Enemy *closetEnemy = nullptr;
	float closestDistance = std::numeric_limits<float>::max();
	float distance;
	for (auto enemy : mEnemies)
	{
		distance = Angazi::Math::Distance(line.from , enemy->GetPosition());
		if (enemy->IsAlive() && Angazi::Math::Intersect(line,enemy->GetBoundingCircle()) && distance < closestDistance)
		{
			closetEnemy = enemy;
			closestDistance = distance;
		}
	}
	if(closetEnemy)
		closetEnemy->TakeDamage(damage);
}

void EnemyManager::CheckPlayerCollision()
{
	Angazi::Math::Circle survivorCircle = survivor.GetBoundingCircle();
	for (auto enemy : mEnemies)
	{
		if (enemy->IsAlive() && Angazi::Math::Intersect(survivorCircle, enemy->GetBoundingCircle())  && enemy->CanDamage())
		{
			survivor.TakeDamage(enemy->GetDamage());
		}
	}
}

void EnemyManager::AlertEnemies()
{
	for (auto enemy : mEnemies)
	{
		enemy->SetAlerted(true);
	}
}


