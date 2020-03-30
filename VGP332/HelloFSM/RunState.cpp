#include "RunState.h"
#include "Survivor.h"

extern Survivor survivor;

using namespace Angazi;

void RunState::Enter(Enemy& agent)
{
}

void RunState::Update(Enemy& agent, float deltaTime)
{
	const Math::Vector2 & targetPos = survivor.GetPosition();
	const Math::Vector2 heading = Math::Normalize(targetPos - agent.GetPosition());
	const Math::Vector2 position = agent.GetPosition() + heading * agent.GetSpeed()* 3.0f *deltaTime;
	agent.SetPosition(position);
}

void RunState::Exit(Enemy& agent)
{

}
