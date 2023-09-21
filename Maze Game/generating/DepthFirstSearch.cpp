#include "DepthFirstSearch.h"
#include "Maze.h"
void DepthFirstSearch::generate()
{
	if (visitedCells == maze->getCellCount())
	{
		generationFinished = true;
		return;
	}
	std::vector<int> neighbours = maze->getNeighbours(q.back());
	while (neighbours.empty())
	{
		q.pop_back();
		maze->cells[q.back()] |= Maze::CellState::VISITED_TWICE;
		neighbours = maze->getNeighbours(q.back());
	}
	int nextCellIndex = maze->random(neighbours);
	maze->createPath(q.back(), nextCellIndex);
	visitedCells++;
	currentCell = nextCellIndex;
	q.push_back(nextCellIndex);
}

void DepthFirstSearch::reset()
{
	generationFinished = false;
	q.clear();
	q.push_back(startCell);
	currentCell = startCell;
	maze->setCellState(startCell, Maze::VISITED);
	visitedCells = 1;
}
