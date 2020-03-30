#include "FightState.h"

void FightState::Enter(Player & agent)
{

}

void FightState::Update(Player & agent, float deltaTime)
{
	agent.displacement.y = agent.maxSpeed * deltaTime;
	if (agent.GetEnemyDestination().x > agent.position.x)
		agent.SetFacingLeft(false);
	else
		agent.SetFacingLeft(true);
	if (agent.GetCurrentAnimation() != Attacking)
		weaponType = None;
	if (X::Math::Distance(agent.position, agent.GetEnemyDestination()) < agent.GetMeleeRange())
	{
		agent.SetCurrentWeapon(1);
		if (weaponType != Melee)
			agent.ChangeAnimation(Attacking, true);
		agent.Attack();
		weaponType = Melee;
	}
	else
	{
		agent.SetCurrentWeapon(2);
		if (weaponType != Ranged)
			agent.ChangeAnimation(Attacking, true);
		agent.Attack();
		weaponType = Ranged;
	}
}

void FightState::Exit(Player & agent)
{
}
