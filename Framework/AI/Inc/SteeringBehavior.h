#pragma once
#include "Common.h"

namespace Angazi::AI
{
	class Agent;

	class SteeringBehavior
	{
	public:
		SteeringBehavior() = default;
		virtual ~SteeringBehavior() = default;

		virtual Math::Vector2 Calculate(Agent& agent) = 0;

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive; }

		virtual void DebugDraw(Agent& agent) {}

	private:
		bool mActive = false;
	};
}