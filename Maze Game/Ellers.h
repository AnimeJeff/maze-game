#pragma once
#include "GenerationAlgorithm.h"
class Ellers : public GenerationAlgorithm
{
public:
	Ellers(Maze* maze, int startCell) : GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Eller's Algorithm";
	}

	void generate() override;
	void reset() override;
};

