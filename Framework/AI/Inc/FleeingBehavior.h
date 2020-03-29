#pragma once
#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class FleeingBehavior : public SteeringBehavior
	{
	public:
		FleeingBehavior() = default;
		virtual ~FleeingBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		Math::Vector2 Flee(Agent& agent, Math::Vector2 fleePosition);

	};
}