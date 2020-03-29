#pragma once
#include "SeekingBehavior.h"

namespace Angazi::AI
{
	class PursuitBehavior : public SeekingBehavior
	{
	public:
		PursuitBehavior() = default;
		virtual ~PursuitBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		void DebugDraw(Agent &agent) override;

		Agent* evader;
		Math::Vector2 target;
	};
}