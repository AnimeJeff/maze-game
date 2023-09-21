#include "AStar.h"
#include "Maze.h"
#include <limits>
int AStar::calculateManhattanDistance(int cellIndex)
{
	return int((end - cellIndex) / maze->getCols()) + int((end - cellIndex) % maze->getCols());
}


void AStar::reset()
{
	found = false;
	cameFrom.clear();
	path.clear();
	closed.clear();
	fScore.clear();
	gScore.clear(); 
	openSet = std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>>();
}

void AStar::find()
{
	if (start < 0 || end < 0) return; // invalid start and end
	reset();
	for (int i = 0; i < maze->getCellCount(); i++)
	{ 
		gScore[i] = std::numeric_limits<int>::max(); 
		fScore[i] = std::numeric_limits<int>::max();
	};

	fScore[start] = calculateManhattanDistance(start);
	gScore[start] = 0;
	cameFrom[start] = start;
	openSet.push(std::make_pair(fScore[start], start));
	closed.insert(start);

	while (!openSet.empty())
	{
		int currentCellIndex = openSet.top().second;
		if (currentCellIndex == end)
		{
			constructPath();
			return;
		}
		openSet.pop();
		int costSoFar = gScore[currentCellIndex];
		for (auto neighbour : getNeighbours(currentCellIndex))
		{
			if (costSoFar + 1 < gScore[neighbour])
			{
				gScore[neighbour] = costSoFar + 1;
				fScore[neighbour] = gScore[neighbour] + calculateManhattanDistance(neighbour);
				cameFrom[neighbour] = currentCellIndex;
				if (!closed.contains(neighbour))
				{
					openSet.push(std::make_pair(fScore[neighbour], neighbour));
					closed.insert(neighbour);
				}
			}
		}
	}
	std::cout << "failed to reach end" << std::endl;
}
