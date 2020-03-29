#include "Precompiled.h"
#include "SeekingBehavior.h"
#include "Agent.h"

using namespace Angazi;
using namespace Angazi::AI;

Math::Vector2 SeekingBehavior::Calculate(Agent & agent)
{
	return Seek(agent, agent.destination);
}

Math::Vector2 SeekingBehavior::Seek(Agent & agent, Math::Vector2 destination)
{
	Math::Vector2 desiredVelocity = Math::Normalize(destination - agent.position) * agent.maxSpeed;
	return desiredVelocity - agent.velocity;
}