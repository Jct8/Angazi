#include "Precompiled.h"
#include "Agent.h"
#include "AIWorld.h"
#include "ObstacleAvoidanceBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;
using namespace Angazi::Graphics;

Math::Vector2 ObstacleAvoidance::Calculate(Agent & agent)
{
	AIWorld::Obstacles obstacles = agent.world.GetObstacles();
	boxLength = boxExtend + Math::Magnitude(agent.velocity) / agent.maxSpeed * boxExtend;

	Math::Matrix3 localToWorld = agent.LocalToWorld();
	Math::Matrix3 worldToLocal = Math::Inverse(agent.LocalToWorld());
	Math::Circle closestObstacle;
	Math::Vector2 localPositionOfObject; 
	float closestDistance = FLT_MAX;
	bool found = false;

	for (auto obstacle : obstacles)
	{
		if (Math::Distance(obstacle.center ,agent.position) - obstacle.radius > boxLength )
			continue;

		auto localPosition = Math::TransformCoord(obstacle.center, worldToLocal);
		if (localPosition.y >= 0)
		{
			float newRadius = obstacle.radius + agent.radius;
			if (Math::Abs(localPosition.x) < newRadius)
			{
				float intersectionY = localPosition.y - sqrt(newRadius*newRadius - localPosition.x * localPosition.x);
				if (intersectionY <= 0.0f)
					intersectionY = localPosition.y + sqrt(newRadius*newRadius - localPosition.x * localPosition.x);

				if (intersectionY < closestDistance)
				{
					closestDistance = intersectionY;
					closestObstacle = obstacle;
					localPositionOfObject = localPosition;
					found = true;
				}
			}
		}
	}

	if (found)
	{
		Math::Vector2 force;
		float multiplier = lateralForceScale + (boxLength - localPositionOfObject.y) / boxLength;
		force.x = (closestObstacle.radius - localPositionOfObject.x) * multiplier;

		force.y = (closestObstacle.radius - localPositionOfObject.y) * brakingWeight;
		return Math::TransformNormal(force, localToWorld);
	}
	return Math::Vector2();
}


void ObstacleAvoidance::DebugDraw(Agent& agent)
{
	Math::Matrix3 localToWorld = agent.LocalToWorld();
	Math::Vector2 boxExtendTLWorld = Math::TransformCoord({ agent.radius,0.0f }, localToWorld);
	Math::Vector2 boxExtendTRWorld = Math::TransformCoord({ agent.radius , boxLength }, localToWorld);
	Math::Vector2 boxExtendBLWorld = Math::TransformCoord({ -agent.radius,0.0f }, localToWorld);
	Math::Vector2 boxExtendBRWorld = Math::TransformCoord({ -agent.radius ,  boxLength }, localToWorld);
	SimpleDraw::AddScreenLine({ boxExtendTLWorld }, { boxExtendTRWorld }, Colors::AliceBlue);
	SimpleDraw::AddScreenLine({ boxExtendBLWorld }, { boxExtendBRWorld }, Colors::AliceBlue);
	SimpleDraw::AddScreenLine({ boxExtendTLWorld }, { boxExtendBLWorld }, Colors::AliceBlue);
	SimpleDraw::AddScreenLine({ boxExtendTRWorld }, { boxExtendBRWorld }, Colors::AliceBlue);
}