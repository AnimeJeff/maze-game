#pragma once
#include "GenerationAlgorithm.h"
class Wilsons : public GenerationAlgorithm
{
public:
	Wilsons(Maze* maze, int startCell) : GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Wilson's Algorithm";
	}
	void generate() override;
	void reset() override;
};

