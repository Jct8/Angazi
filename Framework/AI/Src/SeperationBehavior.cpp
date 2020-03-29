#include "Precompiled.h"
#include "SeperationBehavior.h"
#include "Agent.h"

using namespace Angazi;
using namespace Angazi::AI;

Math::Vector2 SeperationBehavior::Calculate(Agent & agent)
{
	Math::Vector2 totalForce;
	for (auto& neighbor : agent.neighbors)
	{
		auto neighborToAgent = agent.position - neighbor->position;
		auto distanceToAgent = Math::Magnitude(neighborToAgent);
		if (distanceToAgent <= 0.0f)
		{
			totalForce += agent.heading *agent.maxSpeed;
		}
		else
		{
			auto seperationDirection = neighborToAgent / distanceToAgent;
			auto seperationForce = agent.maxSpeed * (1.0f - Math::Min(distanceToAgent, 100.0f) / 100.0f);
			totalForce += seperationDirection * seperationForce;
		}
	}

	return totalForce;
}
