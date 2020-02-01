#pragma once
#include "Graph.h"

namespace Angazi::AI
{
	class Dijkstras
	{
	public:
		Path Search(const Graph & Graph, const Coord& start, const Coord& end, std::function<bool(Coord)> isBlocked, std::function<float(Coord, Coord)> getCost);

		const std::list<Coord> GetClosedList() const { return closedList; }
		const std::vector<Coord> GetParents() const { return parent; }

	private:
		std::list<Coord> openList;
		std::list<Coord> closedList;
		std::vector<Coord> parent;
		std::vector<float> g;
		std::vector<bool> opened;
		std::vector<bool> closed;

	};
}