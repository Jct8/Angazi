#pragma once

#include "SeekingBehavior.h"

namespace Angazi::AI
{
	class WanderBehavior : public SeekingBehavior
	{
	public:
		WanderBehavior() = default;
		virtual ~WanderBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		void DebugDraw(Agent& agent) override;

		float wanderRadius = 50.2f;
		float wanderDistance = 50.0f;
		float wanderJitter = 30.2f;
	private:
		Math::Vector2 centerWorld;
		Math::Vector2 targetWorld;
		Math::Vector2 wanderTarget = { wanderRadius * cosf(1.0f), wanderRadius * sinf(1.0f) };
	};
}