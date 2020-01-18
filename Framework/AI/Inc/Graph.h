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

	void Graph::Resize(int columns, int rows)
	{
		mColumns = columns;
		mRows = rows;
		mNodes.resize(columns * rows);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < columns; ++x)
			{
				//Homework: Connect Neighbours to each node (up to 8, maybe less)
			}
		}
	}

	Graph::Node* Graph::GetNode(Coord coord)
	{
		// Homework: Do bound check , then either return & of node, or nullptr;
	}

	int Graph::GetColumns() const
	{
		return mColumns;
	}
	int Graph::GetRows() const
	{
		return mRows;
	}

	int Graph::GetIndex(Coord coord) const
	{
		return coord.x + (coord.y * mColumns);
	}
}