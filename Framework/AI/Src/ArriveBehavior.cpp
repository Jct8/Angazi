#include "Precompiled.h"

#include "Agent.h"
#include "ArriveBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;

Angazi::Math::Vector2 ArriveBehavior::Calculate(Agent & agent)
{
	return Arrive(agent, agent.destination);
}

Math::Vector2 AI::ArriveBehavior::Arrive(Agent & agent, Math::Vector2 destination)
{
	auto direction = destination - agent.position;
	auto distance = Math::Magnitude(direction);
	if (distance > 0.0f)
	{
		float speed = Math::Min(distance / (deceleration * decelerationTweaker), agent.maxSpeed);
		Math::Vector2 desiredVelocity = direction * speed / distance;

		return desiredVelocity - agent.velocity;
	}
	return Math::Vector2();
}
