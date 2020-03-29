#include "Precompiled.h"
#include "Agent.h"
#include "FleeingBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;

Math::Vector2 FleeingBehavior::Calculate(Agent & agent)
{
	return Flee(agent, agent.destination);
}

Math::Vector2 AI::FleeingBehavior::Flee(Agent & agent, Math::Vector2 fleePosition)
{
	Math::Vector2 desiredVelocity = Math::Normalize(agent.position - fleePosition) * agent.maxSpeed;
	return desiredVelocity - agent.velocity;
}
