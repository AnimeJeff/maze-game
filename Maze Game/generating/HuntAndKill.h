#pragma once
#include "GenerationAlgorithm.h"

class HuntAndKill : public GenerationAlgorithm
{
public:
	HuntAndKill(Maze* maze, int startCell) :GenerationAlgorithm(maze, startCell)
	{
		/*currentCell = startCell;*/
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Hunt and Kill!";
	};
	void generate() override;;
	void reset() override;;
private:
	int firstUnvisitedCell = 0;
	int visitedCells = 0;
};

