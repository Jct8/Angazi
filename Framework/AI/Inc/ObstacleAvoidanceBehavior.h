#pragma once

#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class ObstacleAvoidance : public SteeringBehavior
	{
	public:
		ObstacleAvoidance() = default;
		virtual ~ObstacleAvoidance() = default;
		void DebugDraw(Agent& agent) override;

		Math::Vector2 Calculate(Agent& agent) override;

		float boxExtend = 100.0f;
		float brakingWeight = 2.8f;
		float lateralForceScale = 2.0f;
	private:
		float boxLength;
	};
}