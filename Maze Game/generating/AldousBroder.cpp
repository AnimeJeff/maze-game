#include "AldousBroder.h"
#include "Maze.h"

void AldousBroder::generate() 
{
	if (visitedCells == maze->getCellCount())
	{
		generationFinished = true;
		return;
	}
	std::vector<int> neighbours = maze->getAllNeighbours(currentCell);
	int nextCellIndex = maze->random(neighbours);
	if (!maze->checkCellState(nextCellIndex, Maze::VISITED))
	{
		maze->createPath(currentCell, nextCellIndex);
		visitedCells++;
	}
	currentCell = nextCellIndex;
}

void AldousBroder::reset()
{
	generationFinished = false;
	currentCell = startCell;
	maze->setCellState(startCell, Maze::VISITED);
	visitedCells = 1;
}
