#include "Prims.h"
#include "Maze.h"

void Prims::generate() {
	if (frontiers.empty()) {
		generationFinished = true;
		return;
	}
	int frontier;
	std::vector<int> neighbours;
	int rnd = maze->random(frontiers.size());
	// getting a random frontier
	std::set<int>::iterator it = frontiers.begin();
	std::advance(it, rnd);
	frontier = *it;
	currentCell = frontier;
	// removing the frontier
	frontiers.erase(frontier);
	// getting a random neighbour that has been visited before
	neighbours = maze->getNeighbours(frontier, true);
	auto neighbour = neighbours[maze->random(neighbours.size())];
	maze->createPath(neighbour, frontier);
	// adding the frontiers of the selected frontier to the set.
	for (const auto& f : maze->getNeighbours(frontier))
	{
		frontiers.insert(f);
	}

}

void Prims::reset()
{
	generationFinished = false;
	frontiers.clear();
	for (const auto& frontier : maze->getNeighbours(startCell))
	{
		frontiers.insert(frontier);
	}
	maze->cells[startCell] |= Maze::VISITED;
	currentCell = startCell;
}
