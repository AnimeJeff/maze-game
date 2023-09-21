#pragma once
#include "PathfindingAlgorithm.h"
#include <queue>

class BreadthFirstSearch : public PathfindingAlgorithm
{
	std::deque<int> q;
public:
	BreadthFirstSearch(Maze* maze) :PathfindingAlgorithm(maze) {}

	std::string name() const override
	{
		return "Breadth First Search";
	}

	void reset() override;
	void find() override;

};


