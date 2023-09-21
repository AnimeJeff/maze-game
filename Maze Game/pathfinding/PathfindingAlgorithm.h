#pragma once
#include <set>
#include <map>
#include <string>
#include <vector>
#include <set>

class Maze;
class PathfindingAlgorithm
{
protected:
	int start = -1;
	int end = -1;
	Maze* maze;
	std::map<int, int> cameFrom;
	bool found = false;
	std::vector<int> path;
	std::set<int> closed;
	PathfindingAlgorithm(Maze* maze) : maze(maze) {}
protected:
	std::vector<int> getNeighbours(int cellIndex, bool visited = false);
	void constructPath()
	{
		int current = end;
		do
		{
			current = cameFrom[current];
			path.insert(path.begin(), current);
			//maze->cells[current] |= PATH;
		} while (current != start);
	}

public:
	enum Type
	{
		A_STAR,
		BFS,
		DEAD_END_FILLING,
		DFS,
		WALL_FOLLOWER
	};
	virtual void find() = 0;
	virtual std::string name() const = 0;
	virtual void reset() = 0;

	
	void setStart(int start)
	{
		this->start = start;
	}
	void setEnd(int end)
	{
		this->end = end;
	}
	
	std::vector<int> getPath() const
	{
		return path;
	}

};


