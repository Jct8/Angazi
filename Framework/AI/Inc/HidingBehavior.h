#pragma once

#include "ArriveBehavior.h"

namespace Angazi::AI
{
	class HidingBehavior : public ArriveBehavior
	{
	public:
		HidingBehavior() = default;
		virtual ~HidingBehavior() = default;

		Math::Vector2 Calculate(Agent& agent) override;
		float outsideDistance = 20.0f;
	private:
	};
}