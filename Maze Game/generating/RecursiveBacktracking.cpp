#include "RecursiveBacktracking.h"
#include "Maze.h"

void RecursiveBacktracking::generate()
{
	if (stack.empty()) {
		generationFinished = true;
		return;
	}
	auto neighbours = maze->getNeighbours(stack.top());
	if (neighbours.empty())
	{
		stack.pop();
		currentCell = stack.top();
		return;
	}
	int neighbour = maze->random(neighbours);
	maze->createPath(currentCell, neighbour);
	currentCell = neighbour;
	stack.push(neighbour);
}

void RecursiveBacktracking::reset()
{
	currentCell = startCell;
	generationFinished = false;
	stack = std::stack<int>();
	stack.push(startCell);
}
