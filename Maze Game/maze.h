#pragma once
#include <vector>
#include <iostream>

#include <random>

#include <utility>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <assert.h>

#include "DepthFirstSearch.h"
#include "Prims.h"
#include "RecursiveBacktracking.h"
#include "HuntAndKill.h"
#include "Kruskals.h"
#include "AldousBroder.h"
#include "BinaryTree.h"

#include "AStar.h"
#include "BreadthFirstSearch.h"

class Maze
{
private:
	friend DepthFirstSearch;
	friend Prims;
	friend RecursiveBacktracking;
	friend HuntAndKill;
	friend Kruskals;
	friend AldousBroder;
	friend BinaryTree;

	std::shared_ptr<GenerationAlgorithm> generator = std::make_unique<DepthFirstSearch>(this, generationStartCell);

	int random(int max = 10000) {
		assert(max <= 10000);
		return intdist(mt) % max;
	}


	int random(std::vector<int>& list) {
		int out;
		std::sample(list.begin(), list.end(), &out, 1, mt);
		return out;
	}


	std::vector<int> getAllNeighbours(int cellIndex)
	{
		std::vector<int> neighbours;
		int northCellIndex = cellIndex - cols;
		int southCellIndex = cellIndex + cols;
		int westCellIndex = cellIndex - 1;
		int eastCellIndex = cellIndex + 1;

		if (northCellIndex >= 0)
		{
			neighbours.push_back(northCellIndex);
		}

		if (southCellIndex < cells.size())
		{
			neighbours.push_back(southCellIndex);
		}

		if (cellIndex % cols != 0 && westCellIndex >= 0)
		{
			neighbours.push_back(westCellIndex);
		}

		if (cellIndex % cols != cols - 1 && eastCellIndex < cells.size())
		{
			neighbours.push_back(eastCellIndex);
		}

		return neighbours;
	}
	bool isInvalidIndex(int cellIndex) const {
		return cellIndex < 0 || cellIndex >= cells.size();
	}
public:
	
	void setCellState(int cellIndex, int state)
	{
		if (isInvalidIndex(cellIndex)) return;
		if (state < 0 || state > 512) return;
		cells[cellIndex] |= state;
	}
	
	void unsetCellState(int cellIndex, int state)
	{
		if (isInvalidIndex(cellIndex)) return;
		if (state < 0 || state > 512) return;
		cells[cellIndex] &= ~state;
	}
	
	bool checkCellState(int cellIndex, int state) const
	{
		if (isInvalidIndex(cellIndex)) return false;
		if (state < 0 || state > 512) return false;
		return (cells[cellIndex] & state) == state;
	}

	int generationStartCell;
	void setGenerationAlgorithm(GenerationAlgorithm::Type algorithm)
	{
		//std::cout << algorithm;
		switch (algorithm)
		{
		case GenerationAlgorithm::RECURSIVE_BACKTRACKING:
			generator = std::make_unique<RecursiveBacktracking>(this, generationStartCell);
			break;
		case GenerationAlgorithm::DFS:
			generator = std::make_shared<DepthFirstSearch>(this, generationStartCell);
			break;
		case GenerationAlgorithm::KRUSKALS:
			generator = std::make_unique<Kruskals>(this, generationStartCell);
			break;
		case GenerationAlgorithm::PRIMS:
			generator = std::make_shared<Prims>(this, generationStartCell);
			break;
		case GenerationAlgorithm::HUNT_AND_KILL:
			generator = std::make_shared<HuntAndKill>(this, generationStartCell);
			break;
		case GenerationAlgorithm::ALDOUS_BRODER:
			generator = std::make_unique<AldousBroder>(this, generationStartCell);
			break;
		case GenerationAlgorithm::BINARY_TREE:
			generator = std::make_unique<BinaryTree>(this, generationStartCell);
			break;
		default:
			break;
		}
		reset();
	}

	std::string currentGenerationAlgorithmName() const
	{
		return generator->name();
	}

	int getCurrentGenerationCell() {
		return generator->getCurrentCell();
	}

	void generate() {
		generator->generate();
	}

	bool finishedGenerating() const
	{
		return generator->finishedGenerating();
	}


private:
	std::unique_ptr<PathfindingAlgorithm> pathfinder = std::make_unique<AStar>(this);

public:
	std::vector<int> getPath() const
	{
		return pathfinder->getPath();
	}

	inline std::string currentPathfindingAlgorithmName()
	{
		return pathfinder->name();
	}

	void setPathfindingAlgorithm(PathfindingAlgorithm::Type type)
	{
		switch (type)
		{
		case PathfindingAlgorithm::ASTAR:
			pathfinder = std::make_unique<AStar>(this);
			break;
		case PathfindingAlgorithm::BFS:
			pathfinder = std::make_unique<BreadthFirstSearch>(this);
			break;
		}
	}

	
	void setPathfindingStartCell(int cellIndex)
	{
		if (cellIndex < 0 || cellIndex >= cells.size()) return;
		pathfindingStartCell = cellIndex;
		pathfinder->setStart(pathfindingStartCell);
	}
	
	void unsetPathfindingStartCell()
	{
		pathfindingStartCell = -1;
		pathfinder->setStart(pathfindingStartCell);
	}
	void unsetPathfindingEndCell()
	{
		pathfindingEndCell = -1;
		pathfinder->setEnd(pathfindingEndCell);
	}
	
	void setPathfindingStartCell(int row, int col)
	{
		setPathfindingStartCell(getCellIndex(row, col));
	}
	
	void setPathfindingEndCell(int cellIndex)
	{
		if (cellIndex < 0 || cellIndex >= cells.size()) return;
		pathfindingEndCell = cellIndex;
		pathfinder->setEnd(pathfindingEndCell);
	}
	void setPathfindingEndCell(int row, int col)
	{
		setPathfindingEndCell(getCellIndex(row, col));
	}

	bool hasPathfindingStartCell() {
		return pathfindingStartCell > 0;
	}
	bool hasPathfindingEndCell() {
		return pathfindingEndCell > 0;
	}

	bool isPathfindingStartCell(int cellIndex) {
		return pathfindingStartCell == cellIndex;
	}
	bool isPathfindingEndCell(int cellIndex) {
		return pathfindingEndCell == cellIndex;
	}
	bool isPathfindingPathCell(int cellIndex) {
		return (cells[cellIndex] & PATH) == PATH;
	}

	void find() {
		pathfinder->find();
	}

public:
	Maze(unsigned int rows, unsigned int cols, unsigned int cellSize)
		:rows(rows), cols(cols), cellSize(cellSize), cells(rows* cols), cellCount(cells.size())
	{
		setSearchStart(0, 0);
	}
	
	
	int getCellIndex(int row, int col) const
	{
		size_t index = row * cols + col;
		if (index > cells.size()) 
		{
			throw std::exception("index out of bound!");
		}
		return index;
	}
	
	int getCellIndexFromMousePos(sf::Vector2i mousePos) const
	{
		int row = int(mousePos.y / cellSize);
		int col = int(mousePos.x / cellSize);
		return getCellIndex(row, col);
	}
	
	sf::Vector2i getRowColFromMousePos(sf::Vector2i mousePos) const
	{
		int row = (int) floor(mousePos.y / cellSize);
		int col = (int) floor(mousePos.x / cellSize);
		return sf::Vector2i(row, col);
	}
	
	
	
	
	void setDimensions(int rows, int cols) {
		this->rows = rows;
		this->cols = cols;
		cellCount = rows * cols;
		cells.resize(cellCount);

		reset();
	}
	
	sf::Vector2i getDimensions() {
		return sf::Vector2i(rows, cols);
	}
	
	int getRows() const {
		return rows;
	}
	int getCols() const
	{
		return cols;
	}

	
	int getCellCount() const
	{
		return cellCount;
	}

	enum CellPath {
		NORTH_PATH = 0b0000000001,
		EAST_PATH = 0b0000000010,
		SOUTH_PATH = 0b0000000100,
		WEST_PATH = 0b0000001000,
	};
	
	enum CellState {
		CLOSED = 0b0001000000,
		PATH = 0b0010000000,
		VISITED = 0b0100000000,
		VISITED_TWICE = 0b1000000000,
	};
	
	void print_maze();
	void reset();
	void setSearchStart(int row, int col) 
	{
		setSearchStart(getCellIndex(row, col));
	}
	
	void setSearchStart(int cellIndex) // sets the starting cell for the generation algorithm
	{
		generationStartCell = cellIndex;
		if (generator)
		{
			generator->setStartCell(generationStartCell);
		}
		reset();
	}
	
	
	std::vector<int> cells;
private:
	void createPath(int cellIndex1, int cellIndex2) // creates a path between cell 1 and cell 2
	{
		int diff = cellIndex2 - cellIndex1;
		if (-diff == cols) // north path
		{
			cells[cellIndex1] |= VISITED | NORTH_PATH;
			cells[cellIndex2] |= VISITED | SOUTH_PATH;
		}
		else if (diff == cols) // south path
		{
			cells[cellIndex1] |= VISITED | SOUTH_PATH;
			cells[cellIndex2] |= VISITED | NORTH_PATH;
		}
		else if (diff == -1) // west path
		{
			cells[cellIndex1] |= VISITED | WEST_PATH;
			cells[cellIndex2] |= VISITED | EAST_PATH;
		}
		else if (diff == 1) // east path
		{
			cells[cellIndex1] |= VISITED | EAST_PATH;
			cells[cellIndex2] |= VISITED | WEST_PATH;
		}
		else
		{
			std::cout << "can't create path between " << cellIndex1 << " and " << cellIndex2 << std::endl;
		}
	}
	
private:
	std::default_random_engine rd;
	std::uniform_int_distribution<int> intdist{ 0, 1000 };
	std::mt19937 mt{ rd() };
	std::vector<int> getNeighbours(int cellIndex, bool visited = false);

	unsigned int rows;
	unsigned int cols;
	unsigned int cellSize;
	unsigned int cellCount;
	int pathfindingStartCell = -1;
	int pathfindingEndCell = -1;

};

