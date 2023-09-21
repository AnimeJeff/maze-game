#include "HuntAndKill.h"
#include "Maze.h"

void HuntAndKill::generate()
{
	if (visitedCells == maze->getCellCount()) // should be when currentcell is invalid
	{
		generationFinished = true;
		return;
	}
	auto neighbours = maze->getNeighbours(currentCell);
	// if there are neighbours
	if (!neighbours.empty())
	{
		auto neighbour = neighbours[maze->random(neighbours.size())];
		auto rndNeighbour = neighbours[maze->random(neighbours.size())];
		maze->createPath(currentCell, rndNeighbour);
		visitedCells++;
		currentCell = rndNeighbour;
	}
	// if no neighbours
	else
	{
		// hunting for an unvisited cell with a visited neighbour
		for (int i = firstUnvisitedCell; i < maze->getCellCount(); i++)
		{
			bool visited = maze->checkCellState(i, Maze::VISITED);

			if (!visited)
			{
				// checking if any of its neighbours have been visited
				auto visitedNeighbours = maze->getNeighbours(i, true);
				if (!visitedNeighbours.empty())
				{
					firstUnvisitedCell = i + 1;
					auto rndNeighbour = visitedNeighbours[maze->random(visitedNeighbours.size())];
					maze->createPath(rndNeighbour, i);
					visitedCells++;
					currentCell = i;
					return;
				}
			}
		}
		// fails to find target because the starting point can be anywhere
		firstUnvisitedCell = 0;
	}
}

void HuntAndKill::reset()
{
	visitedCells = 0;
	generationFinished = false;
	firstUnvisitedCell = 0;
	currentCell = startCell;
	maze->setCellState(startCell, Maze::VISITED);

	visitedCells++;
}
