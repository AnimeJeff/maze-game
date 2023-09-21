#pragma once
#include "PathfindingAlgorithm.h"
class DeadEndFilling : public PathfindingAlgorithm
{
public:
	DeadEndFilling(Maze* maze) : PathfindingAlgorithm(maze) {}

	std::string name() const override
	{
		return "Dead-End Filling";
	}

	void reset() override;
	void find() override;
};

