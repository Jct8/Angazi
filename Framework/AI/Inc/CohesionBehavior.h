#pragma once
#include "SeekingBehavior.h"

namespace Angazi::AI
{
	class CohesionBehavior : public AI::SeekingBehavior
	{
	public:
		CohesionBehavior() = default;
		virtual ~CohesionBehavior() = default;

		virtual Math::Vector2 Calculate(Agent& agent) override;

	};
}