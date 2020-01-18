#pragma once

#include "Common.h"

namespace Angazi::AI
{
	struct Coord
	{
		int x, y;
	};
	class Graph
	{
	public:
		struct Node
		{
			std::vector<Coord> neighbors;
		};

		void Resize(int columns, int rows);

		Node* GetNode(Coord coord);

		int GetColumns() const;
		int GetRows() const;

	private:
		int GetIndex(Coord coord) const;
		std::vector<Node> mNodes;
		int mColumns = 0;
		int mRows = 0;
	};

	inline void Graph::Resize(int columns, int rows)
	{
		mColumns = columns;
		mRows = rows;
		mNodes.resize(columns * rows);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < columns; ++x)
			{
				//Homework: Connect Neighbours to each node (up to 8, maybe less)
				Node* nodeTL = GetNode({ x - 1 , y - 1 });
				Node* nodeTT = GetNode({ x   , y - 1 });
				Node* nodeTR = GetNode({ x + 1 , y - 1 });

				Node* nodeML = GetNode({ x - 1 , y });
				Node* nodeMR = GetNode({ x + 1 , y });

				Node* nodeBL = GetNode({ x - 1 , y + 1 });
				Node* nodeBM = GetNode({ x   , y + 1 });
				Node* nodeBR = GetNode({ x + 1 , y + 1 });

				if (nodeTL != nullptr)
					mNodes[GetIndex({ x, y })].neighbors.push_back({ x - 1 , y - 1 });
				if (nodeTT != nullptr)
					mNodes[GetIndex({ x, y })].neighbors.push_back({ x    , y - 1 });
				if (nodeTR != nullptr)
					mNodes[GetIndex({ x, y })].neighbors.push_back({ x + 1 , y - 1 });

				if (nodeML != nullptr)
					mNodes[GetIndex({ x  , y })].neighbors.push_back({ x - 1 , y });
				if (nodeMR != nullptr)
					mNodes[GetIndex({ x , y })].neighbors.push_back({ x + 1 , y });

				if (nodeBL != nullptr)
					mNodes[GetIndex({ x  , y })].neighbors.push_back({ x - 1 , y + 1 });
				if (nodeBM != nullptr)
					mNodes[GetIndex({ x     , y })].neighbors.push_back({ x    , y + 1 });
				if (nodeBR != nullptr)
					mNodes[GetIndex({ x  , y })].neighbors.push_back({ x + 1 , y + 1 });
			}
		}
	}

	inline Graph::Node* Graph::GetNode(Coord coord)
	{
		// Homework: Do bound check , then either return & of node, or nullptr;
		if (coord.x < 0 || coord.x >mColumns - 1 || coord.y < 0 || coord.y > mRows - 1)
		{
			return nullptr;
		}
		return &mNodes[GetIndex(coord)];
	}

	inline int Graph::GetColumns() const
	{
		return mColumns;
	}
	inline int Graph::GetRows() const
	{
		return mRows;
	}

	inline int Graph::GetIndex(Coord coord) const
	{
		return coord.x + (coord.y * mColumns);
	}
}