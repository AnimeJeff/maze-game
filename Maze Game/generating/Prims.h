#pragma once
#include "GenerationAlgorithm.h"
#include <set>
#include <vector>
class Prims : public GenerationAlgorithm
{
public:
	Prims(Maze* maze, int startCell) :GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Prim's Algorithm";
	}
	void generate() override;
	void reset() override;
private:
	std::set<int> frontiers;
};


