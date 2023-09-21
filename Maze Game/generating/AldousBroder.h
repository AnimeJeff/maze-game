#pragma once
#include "GenerationAlgorithm.h"

class AldousBroder : virtual public GenerationAlgorithm
{
public:
	AldousBroder(Maze* maze, int startCell) : GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Aldous-Broder Algorithm";
	}
	void generate() override;
	void reset() override;
private:
	int visitedCells = 0;
};

