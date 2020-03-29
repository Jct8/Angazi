#pragma once
#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class SeperationBehavior : public SteeringBehavior
	{
	public:
		SeperationBehavior() = default;
		virtual ~SeperationBehavior() = default;

		virtual Math::Vector2 Calculate(Agent& agent) override;

	};
}