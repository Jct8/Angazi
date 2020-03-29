#pragma once

#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class SeekingBehavior : public SteeringBehavior
	{
	public:
		SeekingBehavior() = default;
		virtual ~SeekingBehavior() = default;

		virtual Math::Vector2 Calculate(Agent& agent) override;
		Math::Vector2 Seek(Agent& agent, Math::Vector2 destination);

	};
}