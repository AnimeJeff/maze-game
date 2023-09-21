#pragma once
#include "GenerationAlgorithm.h"
#include <vector>

class BinaryTree : virtual public GenerationAlgorithm
{
public:
	BinaryTree(Maze* maze, int startCell) :GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Binary Tree Algorithm";
	}
	void generate() override;
	inline std::vector<int> getNorthAndEastCells(int cellIndex);

	void reset() override;
private:
	int visitedCells = 0;
	int endCell = 0;
};


