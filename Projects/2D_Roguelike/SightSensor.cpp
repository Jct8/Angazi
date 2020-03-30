#include "TileMap.h"
#include "SightSensor.h"
#include "Enemy.h"

void SightSensor::Update(AI::Agent & agent, AI::MemoryRecords & memory, float deltaTime)
{
	auto neighbors = agent.world.GetNeighborhood({ agent.position, neighborhoodRadius }, enemyID);
	for (auto &neighbor : neighbors)
	{
		if (X::Math::Distance(neighbor->position, agent.position) > viewRange)
			continue;

		AI::MemoryRecord& record = FindOrCreate(memory, neighbor->GetUniqueId());
		record.properties["lastSeenPosition"] = neighbor->position;
		if (!static_cast<Enemy*>(neighbor)->IsAlive())
			record.lastRecordTime = 0.0f;
		else
			record.lastRecordTime = X::GetTime();
	}
}
