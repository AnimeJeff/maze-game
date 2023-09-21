#pragma once
#include "GenerationAlgorithm.h"
#include <queue>

class DepthFirstSearch : virtual public GenerationAlgorithm
{
public:
	DepthFirstSearch(Maze* maze, int startCell) : GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Depth-First Search";
	}
	
	void generate() override;
	void reset() override;
private:
	std::deque<int> q;
	int visitedCells = 0;
};

