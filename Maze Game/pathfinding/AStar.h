#pragma once
#include "PathfindingAlgorithm.h"
#include <queue>
#

class AStar : public PathfindingAlgorithm
{
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> openSet;
	std::map<int, int> fScore;
	std::map<int, int> gScore;

private:
	int calculateManhattanDistance(int cellIndex);
public:
	AStar(Maze* maze) : PathfindingAlgorithm(maze) {};

	std::string name() const override
	{
		return "A* Search";
	}
	void reset() override;
	void find() override;

};


