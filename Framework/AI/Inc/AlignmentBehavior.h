#pragma once
#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class AlignmentBehavior : public SteeringBehavior
	{
	public:
		AlignmentBehavior() = default;
		virtual ~AlignmentBehavior() = default;

		virtual Math::Vector2 Calculate(Agent& agent) override;

	};
}