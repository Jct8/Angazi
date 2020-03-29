#include "Precompiled.h"
#include "AIWorld.h"

using namespace Angazi;
using namespace Angazi::AI;
using namespace Angazi::Graphics;

namespace
{
	template<class Element>
	std::vector<Element*> GetElements(const PartitionGrid<Entity>& grid, const Math::Circle& range, float cellSize, uint32_t typeId)
	{
		std::vector<Element*> elements;

		int minX = static_cast<int>((range.center.x - range.radius) / cellSize);
		int maxX = static_cast<int>((range.center.x + range.radius) / cellSize);
		int minY = static_cast<int>((range.center.y - range.radius) / cellSize);
		int maxY = static_cast<int>((range.center.y + range.radius) / cellSize);

		minX = Math::Max(minX, 0);
		maxX = Math::Min(maxX, grid.GetColumns() - 1);
		minY = Math::Max(minY, 0);
		maxY = Math::Min(maxY, grid.GetRows() - 1);

		for (int y = minY; y <= maxY; ++y)
		{
			for (int x = minX; x <= maxX; ++x)
			{
				auto& cell = grid.GetCell(x, y);
				for (auto &element : cell )
				{
					if (element->GetTypeId() == typeId)
						elements.push_back(static_cast<Element*>(element));
				}
			}
		}

		return elements;
	}
}

void AIWorld::RegisterEntity(Entity *entity)
{
	mEntities.push_back(entity);
}

void AIWorld::UnregisterEntity(Entity * entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		std::iter_swap(iter, mEntities.end() - 1);
		mEntities.pop_back();
	}
}

bool AI::AIWorld::HasLineOfSite(const Math::Vector2 & start, const Math::Vector2 & end) const
{
	Math::LineSegment line{ start, end };
	for (auto& wall : mWalls)
	{
		if (Math::Intersect(line, wall))
			return false;
	}
	for (auto& obstacle : mObstacles)
	{
		if (Math::Intersect(line, obstacle))
			return false;
	}
	return true;
}

uint32_t AIWorld::GetNextId()
{
	// 00000000 00000000 XXXXXXXX XXXXXXXX
	//                   00101101 10010101
	// 00101101 10010101 << 16
	//
	// 00000000 00000000 00000000 00101010 mNextid = 42
	// ------------------------------------
	// 00101101 10010101 00000000 00101010

	ASSERT(mNextId < UINT32_MAX, "Run out of ids!");
	return mNextId++;
}

void AIWorld::Initialize(const Settings& settings)
{
	mSettings = settings;

	ASSERT(settings.partitionGridSize > 0.0f, "[AIWorld] Partition grid size must be greater than 0");
	const int columns = static_cast<int>(std::ceilf(settings.worldSize.x / settings.partitionGridSize));
	const int rows = static_cast<int>(std::ceilf(settings.worldSize.y / settings.partitionGridSize));
	mPartitionGrid.Resize(columns, rows);
}

void AIWorld::Update()
{
	mPartitionGrid.ClearCells();
	for (auto entity : mEntities)
	{
		const int columns = static_cast<int>(entity->position.x / mSettings.partitionGridSize);
		const int rows = static_cast<int>(entity->position.y / mSettings.partitionGridSize);
		mPartitionGrid.GetCell(columns, rows).push_back(entity);
	}
}

void AIWorld::DebugDraw()
{
	for (auto& obstacle : mObstacles)
		SimpleDraw::AddScreenCircle(obstacle.center, obstacle.radius, Colors::Cyan);
	for (auto& wall : mWalls)
		SimpleDraw::AddScreenLine(wall.from, wall.to, Colors::Cyan);
	for (int y = 0; y <= mPartitionGrid.GetColumns(); ++y)
	{
		Math::Vector2 to = { y* mSettings.partitionGridSize , 0.0f };
		Math::Vector2 from = { y* mSettings.partitionGridSize , static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight()) };
		SimpleDraw::AddScreenLine(to, from, Colors::AliceBlue);
	}
	
	for (int x = 0; x <= mPartitionGrid.GetRows(); ++x)
	{
		Math::Vector2 to = { 0.0f , x* mSettings.partitionGridSize  };
		Math::Vector2 from = { static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth()) , x* mSettings.partitionGridSize };
		SimpleDraw::AddScreenLine(to, from, Colors::AliceBlue);
	}
}

void AIWorld::AddObstacles(const Math::Circle & obstacles)
{
	mObstacles.push_back(obstacles);
}

void AIWorld::AddWall(const Math::LineSegment & wall)
{
	mWalls.push_back(wall);
}

EntityList AIWorld::GetEntities(const Math::Circle & range, uint32_t typeId)
{
	return GetElements<Entity>(mPartitionGrid, range, mSettings.partitionGridSize, typeId);
}

AgentList AIWorld::GetNeighborhood(const Math::Circle & range, uint32_t typeId)
{
	return GetElements<Agent>(mPartitionGrid, range, mSettings.partitionGridSize, typeId);
}
