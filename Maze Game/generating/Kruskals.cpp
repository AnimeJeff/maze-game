#include "Kruskals.h"
#include "Maze.h"

void Kruskals::generate()
{
	if (edges.empty())
	{
		generationFinished = true;
		map.clear();
		return;
	};
	//select a random unvisited cell
	auto cell = edges.back();
	edges.pop_back();
	auto s1 = map.find(cell.first);
	auto s2 = map.find(cell.second);

	if (s1 == map.end() || s2 == map.end())
	{
		// both do not belong to a set so both are unvisited
		if (s1 == map.end() && s2 == map.end())
		{
			maze->setCellState(cell.first, Maze::VISITED);
			maze->createPath(cell.first, cell.second);
			map[cell.first] = map[cell.second] = std::make_shared<std::set<int>>(std::set<int>{cell.first, cell.second});
		}
		else {
			// one of them has a set
			if (s1 == map.end())
			{
				s2->second->insert(cell.first);
				map[cell.first] = s2->second;
			}
			else {
				s1->second->insert(cell.second);
				map[cell.second] = s1->second;
			}
			maze->createPath(cell.first, cell.second);
		}
	}
	else if (map[cell.first] != map[cell.second]) // different sets
	{
		maze->createPath(cell.first, cell.second);
		auto set2 = *map[cell.second];
		for (std::set<int>::iterator it = set2.begin(); it != set2.end(); )
		{
			map[*it] = map[cell.first];
			map[cell.first]->insert(*it);
			it++;
		}
	}

}

void Kruskals::reset()
{
	generationFinished = false;
	currentCell = -1;
	edges.clear();
	for (int i = 0; i < maze->getCellCount(); i++)
	{
		int col = i % maze->getCols();
		if (i >= maze->getCols()) {
			edges.push_back({ i , i - maze->getCols() });
		}
		if (col > 0)
		{
			edges.push_back({ i , i - 1 });
		}
		map.clear();
	}
	std::shuffle(std::begin(edges), std::end(edges), maze->mt);
}
