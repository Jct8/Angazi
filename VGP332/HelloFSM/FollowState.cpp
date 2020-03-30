#include "FollowState.h"
#include "Survivor.h"

extern Survivor survivor;
using namespace Angazi;

void FollowState::Enter(Enemy& agent)
{
}

void FollowState::Update(Enemy& agent, float deltaTime)
{
	const Math::Vector2 & targetPos = survivor.GetPosition();
	const Math::Vector2 heading = Math::Normalize(targetPos - agent.GetPosition());
	const Math::Vector2 position = agent.GetPosition() + heading * agent.GetSpeed() *deltaTime;
	agent.SetPosition(position);
}

void FollowState::Exit(Enemy& agent)
{
}
