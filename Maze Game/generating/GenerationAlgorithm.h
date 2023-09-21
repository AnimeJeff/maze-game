#pragma once
#include <string>

class Maze;

class GenerationAlgorithm
{
public:
	enum Type {
		ALDOUS_BRODER,
		BINARY_TREE,
		DFS,
		ELLERS,
		HUNT_AND_KILL,
		KRUSKALS,
		PRIMS,
		RECURSIVE_BACKTRACKING,
		WILSONS
	};
	
	virtual std::string name() const = 0;
	virtual void generate() = 0;
	virtual void reset() = 0;

	void setStartCell(int startCell)
	{
		this->startCell = startCell;
		currentCell = startCell;
		reset();
	};
	bool finishedGenerating() const 
	{
		return generationFinished;
	}
	int getCurrentCell()
	{
		return currentCell;
	}
protected:
	GenerationAlgorithm(Maze* maze, int startCell) :maze(maze), startCell(startCell) 
	{
		
	}
	Maze* maze;
	int startCell;
	bool generationFinished = false;
	int currentCell = 0;

};
