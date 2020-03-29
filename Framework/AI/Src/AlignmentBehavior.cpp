#include "Precompiled.h"
#include "AlignmentBehavior.h"
#include "Agent.h"

using namespace Angazi::AI;

Angazi::Math::Vector2 AlignmentBehavior::Calculate(Agent & agent)
{
	Angazi::Math::Vector2 averageHeading = agent.heading;
	for (auto& neighbor : agent.neighbors)
	{
		averageHeading += neighbor->heading;
	}
	averageHeading /= (agent.neighbors.size() + 1.0f);
	return averageHeading - agent.heading;
}
