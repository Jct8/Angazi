#include "IdleState.h"

void IdleState::Enter(Player & agent)
{
	agent.ChangeAnimation(Idle);
}

void IdleState::Update(Player & agent, float deltaTime)
{

}

void IdleState::Exit(Player & agent)
{
}
