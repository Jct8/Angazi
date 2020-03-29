#pragma once
#include "Agent.h"
#include "PartitionGrid.h"

namespace Angazi::AI
{
	class AIWorld
	{
	public:
		struct Settings
		{
			Math::Vector2 worldSize;
			float partitionGridSize;
		};

		using Obstacles = std::vector<Math::Circle>;
		using Walls = std::vector<Math::LineSegment>;

		void Initialize(const Settings& settings);
		void Update();

		void RegisterEntity(Entity* entity);
		void UnregisterEntity(Entity* entity);

		void AddObstacles(const Math::Circle& obstacles);
		void AddWall(const Math::LineSegment& wall);

		EntityList GetEntities(const Math::Circle& range, uint32_t typeId);
		AgentList GetNeighborhood(const Math::Circle & range, uint32_t typeId);

		const Obstacles& GetObstacles() const { return mObstacles; }
		const Walls& GetWalls() const {return  mWalls; }

		bool HasLineOfSite(const Math::Vector2&start, const Math::Vector2 &end)const;

		uint32_t GetNextId();

		void DebugDraw();
	private:
		Settings mSettings;
		EntityList mEntities;
		Obstacles mObstacles;
		Walls mWalls;
		PartitionGrid<Entity> mPartitionGrid;

		uint32_t mNextId = 0;
	};
}