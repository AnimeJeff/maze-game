#pragma once
#include "GenerationAlgorithm.h"
class Sidewinder : public GenerationAlgorithm
{
public:
	Sidewinder(Maze* maze, int startCell) : GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Sidewinder Algorithm";
	}
	void generate() override;
	void reset() override;
};



