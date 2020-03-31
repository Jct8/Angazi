#pragma once
#include "Player.h"
#include <Angazi/Inc/Angazi.h>

enum WeaponType
{
	Melee,
	Ranged,
	None
};

class FightState : public Angazi::AI::State<Player>
{
	void Enter(Player& agent) override;
	void Update(Player& agent, float deltaTime) override;
	void Exit(Player& agent) override;

	WeaponType weaponType;
};