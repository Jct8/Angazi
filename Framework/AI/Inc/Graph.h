#pragma once
#include "Common.h"

namespace Angazi::AI
{
	struct Coord
	{
		int x = -1;
		int y = -1;

		bool IsValid() const
		{
			return x != -1 && y != -1;
		}

		bool operator==(const Coord& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
	};

	using Path = std::vector<Coord>;

	class Graph
	{
	public:
		struct Node
		{
			std::vector<Coord> neighbors;
		};

		void Resize(int columns, int rows);

		Node* GetNode(const Coord & coord);
		const Node* GetNode(Coord coord) const;

		int GetColumns() const;
		int GetRows() const;
		int GetIndex(Coord coord) const;

	private:
		std::vector<Node> mNodes;
		int mColumns = 0;
		int mRows = 0;
	};
}