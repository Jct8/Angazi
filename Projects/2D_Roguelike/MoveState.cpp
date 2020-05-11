#include "MoveState.h"
#include "TileMap.h"

using namespace Angazi;

void MoveState::Enter(Player& agent)
{
}

void MoveState::Update(Player& agent, float deltaTime)
{
	float tilesize = TileMap::Get().GetTileSize();
	if (!agent.path.empty() && static_cast<size_t>(agent.currentPath) < agent.path.size())
		agent.destination = 
		{agent.path[agent.currentPath].x*tilesize ,agent.path[agent.currentPath].y*tilesize + tilesize };

	agent.heading = Math::Normalize(agent.destination - agent.position);
	agent.displacement += agent.heading * agent.maxSpeed *deltaTime;
	if (agent.displacement.y > 0.0f)
		agent.displacement.y = agent.GetJumpSpeed() * deltaTime;

	if (Math::Distance(agent.position, agent.destination) < 5.0f)
	{
		agent.currentPath++;
		agent.heading = 0.0f;
	}
}

void MoveState::Exit(Player& agent)
{

}
