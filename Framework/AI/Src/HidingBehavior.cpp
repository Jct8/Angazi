#include "Precompiled.h"
#include "Agent.h"
#include "AIWorld.h"
#include "HidingBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;

Math::Vector2 HidingBehavior::Calculate(Agent & agent)
{
	if (agent.threat)
	{
		AIWorld::Obstacles obstacles = agent.world.GetObstacles();
		Math::Vector2 closestHidingSpot;
		float closestDistance = FLT_MAX;
		for (auto obstacle : obstacles)
		{
			float distance = outsideDistance + obstacle.radius;
			auto direction = Math::Normalize(obstacle.center - agent.threat->position);

			auto currentHidingSpot = direction*distance + obstacle.center;
			float distanceToAgent = Math::DistanceSqr(currentHidingSpot,agent.position);
			if (distanceToAgent < closestDistance)
			{
				closestDistance = distanceToAgent;
				closestHidingSpot = currentHidingSpot;
			}
		}

		if (closestDistance != FLT_MAX)
		{
			return Arrive(agent,closestHidingSpot);
		}

	}
	return Math::Vector2();
}
