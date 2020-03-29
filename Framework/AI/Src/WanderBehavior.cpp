#include "Precompiled.h"
#include "Agent.h"
#include "WanderBehavior.h"

using namespace Angazi;
using namespace Angazi::AI;
using namespace Angazi::Graphics;

Math::Vector2 WanderBehavior::Calculate(Agent & agent)
{
	wanderTarget += {  Math::RandomFloat(-1.0f, 1.0f) * wanderJitter, Math::RandomFloat(-1.0f, 1.0f) * wanderJitter};

	wanderTarget = Math::Normalize(wanderTarget);
	wanderTarget *= wanderRadius;

	Math::Vector2 targetLocal = wanderTarget + Math::Vector2{ 0.0f, wanderDistance };
	targetWorld = Math::TransformCoord(targetLocal, agent.LocalToWorld());

	Math::Vector2 center = targetLocal - wanderTarget;
	centerWorld = Math::TransformCoord(center, agent.LocalToWorld());

	return Seek(agent, targetWorld);// Seek(agent, targetWorld - agent.position);
}

void AI::WanderBehavior::DebugDraw(Agent& agent)
{
	SimpleDraw::AddScreenCircle(targetWorld, 5.0f, Colors::AliceBlue);
	SimpleDraw::AddScreenCircle(centerWorld, wanderRadius, Colors::YellowGreen);
}
