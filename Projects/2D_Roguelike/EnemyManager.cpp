#include "EnemyManager.h"
#include "Player.h"
#include "TileMap.h"
#include "Game.h"
#include "ImGui/Inc/imgui.h"

extern Player player;
extern Game gameState;

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<EnemyManager> sInstance = nullptr;
	const int maxsize = 100;
}

void EnemyManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "EnemyManger already initialized");
	sInstance = std::make_unique<EnemyManager>();
}

void EnemyManager::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

EnemyManager & EnemyManager::Get()
{
	ASSERT(sInstance != nullptr, "No EnemyManger created!");
	return *sInstance;
}

void EnemyManager::Load(AI::AIWorld & world)
{
	char type;
	char fileName[maxsize];

	int total = 0;
	int count = 0;

	FILE* file = nullptr;
	fopen_s(&file, "EnemyList.txt", "r");
	fscanf_s(file, "Total:%d\n", &total);

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%c %s\n", &type, 1, &fileName, maxsize);
		std::string strFileName(fileName);
		std::vector<Math::Vector2> locations = TileMap::Get().GetSpawnLocation(type);
		for (size_t j = 0; j < locations.size(); j++)
		{
			mEnemies.emplace_back(world, 1);
			mEnemies[count].Load(strFileName, true);
			mEnemies[count].SetPosition({ locations[j].x *32.0f, locations[j].y *32.0f });
			mEnemies[count].position = { locations[j].x *32.0f, locations[j].y *32.0f + mEnemies[count].GetHeight() };
			count++;
		}
	}

	fclose(file);
	mTotalDeaths = 0;

	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].mShowDebug = mShowDebug;
	}
}

void EnemyManager::Unload()
{
	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].Unload();
	}
	mEnemies.clear();
	mTotalDeaths = 0;
}

void EnemyManager::Update(float deltaTime)
{
	if (static_cast<size_t>(mTotalDeaths) >= mEnemies.size())
	{
		gameState = GameWin;
		return;
	}

	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].Update(deltaTime);
	}

}

void EnemyManager::Render()
{
	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].Render();
	}
}

Math::Vector2 EnemyManager::CheckCollision(Math::Rect rect, int damage, Math::Vector2 pos)
{
	int closestEnemy = -1;
	float closestDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		if (mEnemies[i].IsAlive() && Math::Intersect(rect, mEnemies[i].GetBoundingBox()))
		{
			if (closestDistance > Math::Distance(pos, mEnemies[i].GetPosition()))
			{
				closestEnemy = i;
				closestDistance = Math::Distance(pos, mEnemies[i].GetPosition());
			}
		}
	}

	if (closestEnemy != -1)
	{
		mEnemies[closestEnemy].TakeDamage(damage);
		return mEnemies[closestEnemy].GetPosition();
	}
	return Math::Vector2::Zero;
}

bool EnemyManager::CheckProjectileCollision(Math::Vector2 point, int damage)
{
	for (size_t i = 0; i < mEnemies.size(); i++)
	{
		if (mEnemies[i].IsAlive() && Math::PointInRect(point, mEnemies[i].GetBoundingBox()))
		{
			mEnemies[i].TakeDamage(damage);
			return true;
		}
	}
	return false;
}

void EnemyManager::DebugUI()
{
	ImGui::Begin("Settings Editor", nullptr);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Enemy:"))
	{
		if (ImGui::Checkbox("Show Debug lines", &mShowDebug))
		{
			for (size_t i = 0; i < mEnemies.size(); i++)
			{
				mEnemies[i].mShowDebug = mShowDebug;
			}
		}
	}
	ImGui::EndGroup();

	ImGui::End();

}