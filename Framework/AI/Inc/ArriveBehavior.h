#pragma once
#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class ArriveBehavior : public SteeringBehavior
	{
	public:
		ArriveBehavior() = default;
		virtual ~ArriveBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		Math::Vector2 Arrive(Agent& agent, Math::Vector2 destination);
		float decelerationTweaker = 0.3f;
		int deceleration  = 3;
	};
}