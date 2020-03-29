#include "Precompiled.h"
#include "VisualSensor.h"
#include "Agent.h"
#include "AIWorld.h"
#include "MemoryRecord.h"

using namespace Angazi::AI;

void VisualSensor::Update(Agent & agent, MemoryRecords & memory, float deltaTime)
{
	auto neighbors = agent.world.GetNeighborhood({ agent.position, neighborhoodRadius },agent.threat->GetTypeId());
	for (auto &neighbor : neighbors)
	{
		if (Math::Distance(neighbor->position, agent.position) > viewRange)
			continue;
		if (Math::Dot(Math::Normalize(neighbor->position-agent.position),agent.heading) < cosf(Math::Constants::DegToRad*viewAngle))
			continue;
		if (!agent.world.HasLineOfSite(agent.position,neighbor->position))
			continue;

		MemoryRecord& record = FindOrCreate(memory, neighbor->GetUniqueId());
		record.properties["lastSeenPosition"] = neighbor->position;
		//std::get<X::Math::Vector2>(record.properties["lastSeenPosition"]) = neighbor->position;
		record.lastRecordTime = 0.0f;//X::GetTime())
	}
}
