#pragma once
#include "GenerationAlgorithm.h"
#include <stack>

class RecursiveBacktracking : public GenerationAlgorithm
{
public:
	RecursiveBacktracking(Maze* maze, int startCell) :GenerationAlgorithm(maze, startCell)
	{
		setStartCell(startCell);
	}
	std::string name() const override
	{
		return "Recursive Backtracking";
	}
	void generate() override;
	void reset() override;
private:
	std::stack<int> stack;
};

