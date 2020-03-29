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
		Math::Vector2 boxExtend = {30.0f,100.0f};
		float brakingWeight = 0.8f;
		float lateralForceScale = 5.0f;
	private:
		float boxLength;
	};
}