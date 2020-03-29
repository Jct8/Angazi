#include "Precompiled.h"
#include "SteeringModule.h"

using namespace Angazi;
using namespace Angazi::AI;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{}

Math::Vector2 SteeringModule::Calculate()
{
	Math::Vector2 total;
	for (auto& [name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
			total += behavior->Calculate(mAgent);
	}
	return total;
}

void SteeringModule::DebugDraw()
{
	for (auto&[name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
			behavior->DebugDraw(mAgent);
	}
}
