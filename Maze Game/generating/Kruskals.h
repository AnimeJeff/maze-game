#pragma once
#include "GenerationAlgorithm.h"
#include <unordered_map>
#include <memory>
#include <set>

class Kruskals : public GenerationAlgorithm
{
public:
	Kruskals(Maze* maze, int startCell) :GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Randomised Kruskal's Algorithm";
	}
	void generate() override;
	void reset() override;
	std::unordered_map<int, std::shared_ptr<std::set<int>>> map; //
	std::vector<std::pair<int, int>> edges;
	int currentSetCount = 0;
private:

};


