#include "Precompiled.h"
#include "CohesionBehavior.h"
#include "Agent.h"

using namespace Angazi;
using namespace Angazi::AI;

Math::Vector2 CohesionBehavior::Calculate(Agent & agent)
{
	Math::Vector2 averagePosition;
	for (auto &neighbor : agent.neighbors)
	{
		averagePosition += neighbor->position;
	}
	if (agent.neighbors.size() > 0)
	{
		auto target = averagePosition / static_cast<float>(agent.neighbors.size());
		if (target.x == agent.position.x && target.y == agent.position.y) 
			return agent.heading *agent.maxSpeed;
		else
			return Seek(agent,target );
	}
	else
		return Math::Vector2();
}
