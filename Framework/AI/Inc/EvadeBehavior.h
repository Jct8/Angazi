#pragma once
#include "FleeingBehavior.h"

namespace Angazi::AI
{
	class EvadeBehavior : public FleeingBehavior
	{
	public:
		EvadeBehavior() = default;
		virtual ~EvadeBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		void DebugDraw(Agent &agent) override;

		Agent* pursuer;
		Math::Vector2 target;
	};
}