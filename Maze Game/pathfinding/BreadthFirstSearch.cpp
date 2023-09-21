#include "BreadthFirstSearch.h"
#include "Maze.h"

void BreadthFirstSearch::reset()
{
	found = false;
	cameFrom.clear();
	path.clear();
	q.clear();
	closed.clear();
}

void BreadthFirstSearch::find()
{
	if (start < 0 || end < 0) return;
	reset();
	q.push_back(start);
	closed.insert(start);
	while (!q.empty())
	{
		int current = q.front();
		q.pop_front();
		auto neighbours = getNeighbours(current);
		for (auto neighbour : neighbours)
		{
			cameFrom[neighbour] = current;
			q.push_back(neighbour);
			closed.insert(neighbour);
			if (neighbour == end) {
				goto FOUNDLABEL;
			};
		}
	}
FOUNDLABEL:
	constructPath();

}
