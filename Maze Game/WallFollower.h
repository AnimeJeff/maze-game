#pragma once
#include "PathfindingAlgorithm.h"
class WallFollower : public PathfindingAlgorithm
{
public:
	WallFollower(Maze* maze) : PathfindingAlgorithm(maze) {}

	std::string name() const override
	{
		return "Wall Follower";
	}

	void reset() override;
	void find() override;
};

