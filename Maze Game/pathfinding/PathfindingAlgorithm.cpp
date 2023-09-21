#include "PathfindingAlgorithm.h"
#include "Maze.h"

std::vector<int> PathfindingAlgorithm::getNeighbours(int cellIndex, bool visited)
{
	std::vector<int> neighbours;
	int northCellIndex = cellIndex - maze->getCols();
	int southCellIndex = cellIndex + maze->getCols();
	int westCellIndex = cellIndex - 1;
	int eastCellIndex = cellIndex + 1;

	if (maze->checkCellState(cellIndex, Maze::NORTH_PATH) && closed.contains(northCellIndex) == visited)
	{
		neighbours.push_back(northCellIndex);
	}
	if (maze->checkCellState(cellIndex, Maze::SOUTH_PATH) && closed.contains(southCellIndex) == visited)
	{
		neighbours.push_back(southCellIndex);
	}
	if (maze->checkCellState(cellIndex, Maze::WEST_PATH) && closed.contains(westCellIndex) == visited)
	{
		neighbours.push_back(westCellIndex);
	}
	if (maze->checkCellState(cellIndex, Maze::EAST_PATH) && closed.contains(eastCellIndex) == visited)
	{
		neighbours.push_back(eastCellIndex);
	}
	return neighbours;

}
