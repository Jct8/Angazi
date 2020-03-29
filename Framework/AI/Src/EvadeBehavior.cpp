#include "Precompiled.h"
#include "Agent.h"
#include "EvadeBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;
using namespace Angazi::Graphics;

Math::Vector2 EvadeBehavior::Calculate(Agent & agent)
{
	if (agent.threat == nullptr)
		return Math::Vector2();

	auto direction = agent.threat->position - agent.position;

	auto lookAheadTime = Math::Magnitude(direction) / (agent.maxSpeed + Math::Magnitude(agent.threat->velocity));
	return target = Flee(agent, agent.threat->position + agent.threat->velocity * lookAheadTime);

}

void EvadeBehavior::DebugDraw(Agent & agent)
{
	SimpleDraw::AddScreenCircle(target, 3.0f, Colors::AliceBlue);
}
