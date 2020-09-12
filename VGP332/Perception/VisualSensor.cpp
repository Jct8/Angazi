#include "VisualSensor.h"

#include "Mineral.h"
#include "TypeIds.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void VisualSensor::Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime)
{
	auto fovStart = Math::Rotate(agent.heading * viewRange, viewAngle * -0.5f);
	auto fovEnd = Math::Rotate(agent.heading * viewRange, viewAngle * 0.5f);
	SimpleDraw::AddScreenLine(agent.position, agent.position + fovStart, Colors::Cyan);
	SimpleDraw::AddScreenLine(agent.position, agent.position + fovEnd, Colors::Cyan);

	auto angle = atan2(agent.heading.y, agent.heading.x);
	SimpleDraw::AddScreenArc(agent.position, viewRange, angle - viewAngle * 0.5f, angle + viewAngle * 0.5f, Colors::LightCyan);

	auto minerals = agent.world.GetEntities({ agent.position, viewRange }, TypeIds::Mineral);

	auto viewRangeSqr = Math::Sqr(viewRange);
	auto cosViewAngle = cos(viewAngle * 0.5f);
	for (auto& mineral : minerals)
	{
		if (Math::DistanceSqr(agent.position, mineral->position) > viewRangeSqr)
		{
			static_cast<Mineral*>(mineral)->Unseen();
			continue;
		}

		if (Math::Dot(agent.heading, Math::Normalize(mineral->position - agent.position)) < cosViewAngle)
		{
			static_cast<Mineral*>(mineral)->Unseen();
			continue;
		}

		// Have I seen this before? (i.e. do I have a memory record for this entity)
		auto iter = memory.begin();
		for (; iter != memory.end(); ++iter)
			if (iter->entityId == mineral->GetTypeId())
				break;

		static_cast<Mineral*>(mineral)->Seen();
		if (iter != memory.end())
		{
			// If record already exists, just update date
			iter->lastRecordTime = Core::TimeUtil::GetTime();
			iter->properties["lastSeenLocation"] = mineral->position;
		}
		else
		{
			// Otherwise create a new record for this entity
			auto& newRecord = memory.emplace_back();
			newRecord.entityId = mineral->GetTypeId();
			newRecord.lastRecordTime = Core::TimeUtil::GetTime();
			newRecord.properties["lastSeenLocation"] = mineral->position;
		}
	}
}