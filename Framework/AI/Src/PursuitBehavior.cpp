#include "Precompiled.h"
#include "Agent.h"
#include "PursuitBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;
using namespace Angazi::Graphics;

Math::Vector2 PursuitBehavior::Calculate(Agent & agent)
{
	if (agent.threat == nullptr)
		return Math::Vector2();

	auto direction = agent.threat->position - agent.position;
	float relativeHeading = Math::Dot(agent.heading, agent.threat->heading);

	if (Math::Dot(direction, agent.heading) > 0.0f && relativeHeading < -0.95f)
		return target = Seek(agent, agent.threat->position);

	auto lookAheadTime = Math::Magnitude(direction) / (agent.maxSpeed + Math::Magnitude(agent.threat->velocity));
	return target = Seek(agent, agent.threat->position + agent.threat->velocity * lookAheadTime);
}

void PursuitBehavior::DebugDraw(Agent & agent)
{
	SimpleDraw::AddScreenCircle(target, 3.0f, Colors::Yellow);
}
