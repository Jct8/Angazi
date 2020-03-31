#pragma once
#include "Player.h"
#include <Angazi/Inc/Angazi.h>

class MoveState : public Angazi::AI::State<Player>
{
	void Enter(Player& agent) override;
	void Update(Player& agent, float deltaTime) override;
	void Exit(Player& agent) override;
};