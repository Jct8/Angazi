#pragma once
#include "Entity.h"

namespace Angazi::AI
{
	class Agent;

	using AgentList = std::vector<Agent*>;

	class Agent : public Entity
	{
	public:
		Agent(AIWorld& world, uint32_t typeId);
		~Agent() override = default;

		Agent* threat = nullptr;
		AgentList neighbors;
		Math::Vector2 velocity = Math::Vector2::Zero;
		Math::Vector2 destination =Math::Vector2::Zero;

		float maxSpeed = 1.0f;
		float mass = 1.0f;
	};

}