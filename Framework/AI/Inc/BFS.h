#pragma once
#include "Graph.h"

namespace Angazi::AI
{
	class BFS
	{
	public:
		Path Search(const Graph & Graph, const Coord& start, const Coord& end);

		const std::list<Coord> GetClosedList() const { return closedList; }
		const std::vector<Coord> GetParent() const { return parent; }

	private:
		std::list<Coord> openList;
		std::list<Coord> closedList;
		std::vector<Coord> parent;
		std::vector<bool> opened;
		std::vector<bool> closed;

	};
}