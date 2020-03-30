#pragma once

#include <AI.h>
#include "Player.h"
#include<XEngine.h>

class MoveState : public AI::State<Player>
{
	void Enter(Player& agent) override;
	void Update(Player& agent, float deltaTime) override;
	void Exit(Player& agent) override;
};