#include "BinaryTree.h"
#include "Maze.h"
void BinaryTree::generate()
{
	if (generationFinished) return;

	auto northAndEastCells = getNorthAndEastCells(currentCell);
	if (!northAndEastCells.empty())
	{
		int northOrEastCell = maze->random(northAndEastCells);
		maze->createPath(currentCell, northOrEastCell);
	}
	currentCell++;
	if (currentCell >= maze->getCellCount())
	{
		currentCell = -1;
	}
	else if (currentCell == startCell)
	{
		generationFinished = true;
		return;
	}
	 
	
}

std::vector<int> BinaryTree::getNorthAndEastCells(int cellIndex)
{
	std::vector<int> cells;
	if (cellIndex < 0 || cellIndex >= maze->getCellCount()) return cells;
	int northCellIndex = cellIndex - maze->getCols();
	int eastCellIndex = cellIndex + 1;

	if (northCellIndex >= 0)
	{
		cells.push_back(northCellIndex);
	}
	if (cellIndex % maze->getCols() != maze->getCols() - 1 && eastCellIndex < maze->getCellCount())
	{
		cells.push_back(eastCellIndex);
	}

	return cells;
}

void BinaryTree::reset()
{
	generationFinished = false;
	currentCell = startCell;
	endCell = startCell;
	maze->setCellState(startCell, Maze::VISITED);
	visitedCells = 1;
}
