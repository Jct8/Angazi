#include "EnemyManager.h"
#include "Player.h"
#include "TileMap.h"
#include "GameState.h"
#include "ImGui/Inc/imgui.h"

extern Player player;
extern GameState gameState;

namespace
{
	EnemyManager* sInstance = nullptr;
	const int maxsize = 100;

}

void EnemyManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "EnemyManger already initialized");
	sInstance = new EnemyManager();
}

void EnemyManager::StaticTerminate()
{
	delete sInstance;
	sInstance = nullptr;
}

EnemyManager & EnemyManager::Get()
{
	XASSERT(sInstance != nullptr, "No EnemyManger created!");
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
		std::vector<X::Math::Vector2> locations = TileMap::Get().GetSpawnLocation(type);
		for (int j = 0; j < locations.size(); j++)
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

	for (int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].mShowDebug = mShowDebug;
	}
}

void EnemyManager::Unload()
{
	mEnemies.clear();
	mTotalDeaths = 0;
}

void EnemyManager::Update(float deltaTime)
{
	if (mTotalDeaths >= mEnemies.size())
	{
		gameState = GameWin;
		return;
	}

	for (int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].Update(deltaTime);
	}

}

void EnemyManager::Render()
{
	for (int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i].Render();
	}
}

X::Math::Vector2 EnemyManager::CheckCollision(X::Math::Rect rect, int damage, X::Math::Vector2 pos)
{
	int closestEnemy = -1;
	float closestDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < mEnemies.size(); i++)
	{
		if (mEnemies[i].IsAlive() && X::Math::Intersect(rect, mEnemies[i].GetBoundingBox()))
		{
			if (closestDistance > X::Math::Distance(pos, mEnemies[i].GetPosition()))
			{
				closestEnemy = i;
				closestDistance = X::Math::Distance(pos, mEnemies[i].GetPosition());
			}
		}
	}

	if (closestEnemy != -1)
	{
		mEnemies[closestEnemy].TakeDamage(damage);
		return mEnemies[closestEnemy].GetPosition();
	}
	return X::Math::Vector2::Zero();
}

bool EnemyManager::CheckProjectileCollision(X::Math::Vector2 point, int damage)
{
	for (int i = 0; i < mEnemies.size(); i++)
	{
		if (mEnemies[i].IsAlive() && X::Math::PointInRect(point, mEnemies[i].GetBoundingBox()))
		{
			mEnemies[i].TakeDamage(damage);
			return true;
		}
	}
	return false;
}

void EnemyManager::DebugUI()
{
	ImGui::Begin("Settings Editor", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Enemy:"))
	{
		if (ImGui::Checkbox("Show Debug lines", &mShowDebug))
		{
			for (int i = 0; i < mEnemies.size(); i++)
			{
				mEnemies[i].mShowDebug = mShowDebug;
			}
		}
	}
	ImGui::EndGroup();

	ImGui::End();

}