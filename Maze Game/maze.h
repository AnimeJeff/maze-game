#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <random>
#include <map>
#include <set>
#include <utility>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <assert.h>
#include <stack>
class Maze
{
private:
	int random(int max = 10000) {
		assert(max <= 10000);
		return intdist(mt) % max;
	}
	class Algorithm 
	{
	public:
		Algorithm(Maze* maze, int startCell) :maze(maze), startCell(startCell) {}
		virtual std::string name() = 0;
		virtual void generate() = 0;
		virtual void reset() = 0;
		void setStartCell(int startCell) {
			this->startCell = startCell;
			reset();
		};
	protected:
		Maze* maze;
		int startCell;
	};
	
	class Dfs : virtual public Algorithm
	{
	public:
		Dfs(Maze* maze, int startCell) :Algorithm(maze, startCell)
		{
			setStartCell(startCell);
		}
		std::string name() override
		{
			return "Depth-First Search";
		}
		void generate() override {
			std::vector<int> neighbours = maze->getNeighbours(q.back());
			while (neighbours.empty())
			{
				q.pop_back();
				maze->cells[q.back()] |= CellState::VISITED_TWICE;
				neighbours = maze->getNeighbours(q.back());
			}
			int nextCell = neighbours[maze->random(neighbours.size())];
			maze->createPath(q.back(), nextCell);
			q.push_back(nextCell);
		}
		void reset() override
		{
			q.clear();
			q.push_back(startCell);
			maze->cells[startCell] |= VISITED;
			maze->visitedCellCount++;
		}
	private:
		std::deque<int> q;
		};
	
	class Prims : public Algorithm
	{
	public:
		Prims(Maze* maze, int startCell) :Algorithm(maze, startCell)
		{
			setStartCell(startCell);
		}
		std::string name() override
		{
			return "Prim's Algorithm";
		}
		void generate() override {
			if (frontiers.empty()) {
				maze->visitedCellCount = maze->cellCount;
				return;
			}
			int frontier;
			std::vector<int> neighbours;
			int rnd = maze->random(frontiers.size());
			// getting a random frontier
			std::set<int>::iterator it = frontiers.begin();
			std::advance(it, rnd);
			frontier = *it;
			// removing the frontier
			frontiers.erase(frontier);
			// getting a random neighbour that has been visited before
			neighbours = maze->getNeighbours(frontier, true);
			auto neighbour = neighbours[maze->random(neighbours.size())];
			maze->createPath(neighbour, frontier);
			// adding the frontiers of the selected frontier to the set.
			for (const auto& f : maze->getNeighbours(frontier))
			{
				frontiers.insert(f);
			}
			
			return;
		}
		void reset() override
		{
			frontiers.clear();
			for (const auto& frontier : maze->getNeighbours(startCell))
			{
				frontiers.insert(frontier);
			}
			maze->cells[startCell] |= VISITED;
			maze->visitedCellCount++;
		}
	private:
		std::set<int> frontiers;
	};
	
	class RecursiveBacktracking : public Algorithm
	{
	public:
		RecursiveBacktracking(Maze* maze, int startCell) :Algorithm(maze, startCell)
		{
			setStartCell(startCell);
		}
		std::string name() override
		{
			return "Recursive Backtracking";
		}
		void generate() override
		{
			auto neighbours = maze->getNeighbours(stack.top());
			if (neighbours.empty())
			{
				stack.pop();
				return;
			}
			auto neighbour = neighbours[maze->random(neighbours.size())];
			maze->createPath(stack.top(), neighbour);
			stack.push(neighbour);
		}
		void reset() override
		{
			stack = std::stack<int>();
			stack.push(startCell);
		}
	private:
		std::stack<int> stack;
	};

	class HuntAndKill: public Algorithm
	{
		std::string name() override
		{
			return "Hunt and Kill!";
		};
		void generate() override
		{

		};
		void reset() override 
		{
		};
	};



	friend Dfs;
	friend Prims;
	int generationStartCell;
	std::shared_ptr<Algorithm> generationAlgo = nullptr;

	

public:
	enum AlgorithmType {
		RECURSIVE_BACKTRACKING,
		DFS,
		PRIMS,
		HUNT_AND_KILL
	};
	void setGenerationAlgorithm(AlgorithmType algorithm)
	{
		//std::cout << algorithm;
		switch (algorithm)
		{
		case RECURSIVE_BACKTRACKING:
			generationAlgo = std::make_unique<RecursiveBacktracking>(this, generationStartCell);
			break;
		case DFS:
			generationAlgo = std::make_shared<Dfs>(this, generationStartCell);
			break;
		case PRIMS:
			generationAlgo = std::make_shared<Prims>(this, generationStartCell);
			break;
		default:
			break;
		}
		reset();
	}
	std::string getCurrentGenerationAlgorithm()
	{
		return generationAlgo->name();
	}
public:
	inline void generate() {
		generationAlgo->generate();
	}
	inline bool finishedGenerating()
	{
		return visitedCellCount == cellCount;
	}

	void Bfs();
	void A_Star();


public:
	Maze(int rows, int cols, int cellSize)
		:rows(rows), cols(cols), cellSize(cellSize), cells(rows* cols), cellCount(cells.size())
	{
		setSearchStart(0, 0);
	}
	
	int getCellIndex(int row, int col) const
	{
		int index = row * cols + col;
		if (index > cells.size()) 
		{
			throw std::exception("index out of bounds.");
		}
		return index;
	}
	int getCellIndexFromMousePos(sf::Vector2i mousePos) const
	{
		int row = floor(mousePos.y / cellSize);
		int col = floor(mousePos.x / cellSize);
		return getCellIndex(row, col);
	}
	sf::Vector2i getRowColFromMousePos(sf::Vector2i mousePos) const
	{
		int row = (int) floor(mousePos.y / cellSize);
		int col = (int) floor(mousePos.x / cellSize);
		return sf::Vector2i(row, col);
	}
	int getCellCount() const
	{
		return cellCount;
	}
	int cellAt(int index) const
	{
		return cells[index];
	}
	int cellAt(sf::Vector2i mousePos) const
	{
		return cells[getCellIndexFromMousePos(mousePos)];
	}

	void setDimensions(int rows, int cols) {
		this->rows = rows;
		this->cols = cols;
		cellCount = rows * cols;
	}
	void setDimensions(sf::Vector2i dimensions) {
		setDimensions(dimensions.x, dimensions.y);
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

	void setPathfindingStartCell(int cellIndex)
	{
		cells[cellIndex] |= START_CELL;
		pathfindingStartCell = cellIndex;
	}
	void setPathfindingStartCell(int row, int col)
	{
		setPathfindingStartCell(getCellIndex(row, col));
	}
	void setPathfindingEndCell(int cellIndex)
	{
		cells[cellIndex] |= END_CELL;
		pathfindingEndCell = cellIndex;
	}
	void setPathfindingEndCell(int row, int col)
	{
		setPathfindingEndCell(getCellIndex(row, col));
	}
	
	bool hasPathfindingStartCell() {
		return pathfindingStartCell;
	}
	bool hasPathfindingEndCell() {
		return pathfindingEndCell;
	}

	bool isPathfindingStartCell(int cellIndex) {
		return (cells[cellIndex] & START_CELL) == START_CELL;
	}
	bool isPathfindingEndCell(int cellIndex) {
		return (cells[cellIndex] & END_CELL) == END_CELL;
	}
	bool isPathfindingPathCell(int cellIndex) {
		return (cells[cellIndex] & PATH) == PATH;
	}

	int getVisitedCellCount() {
		return visitedCellCount;
	}


	enum CellPath {
		NORTH_PATH = 0b0000000001,
		EAST_PATH = 0b0000000010,
		SOUTH_PATH = 0b0000000100,
		WEST_PATH = 0b0000001000,
	};
	enum CellState {
		START_CELL = 0b0000010000,
		END_CELL = 0b0000100000,
		CLOSED = 0b0001000000,
		PATH = 0b0010000000,
		VISITED = 0b0100000000,
		VISITED_TWICE = 0b1000000000,
	};
	void print_maze();
	void reset();
	void setSearchStart(int row, int col) // sets the cell to start generating maze with either DFS or BFS
	{
		generationStartCell = getCellIndex(row, col);
		cells[generationStartCell] |= CellState::VISITED;
		visitedCellCount++;
		if (generationAlgo)
		{
			reset();
		}
	}
	
	
	std::vector<int> path;
	std::vector<int> cells;

private:
	inline bool hasPath(int cellIndex, CellState path) {
		return (cells[cellIndex] & path) == path;
	}
	void createPath(int cellIndex1, int cellIndex2) // creates a path between cell 1 and cell 2
	{
		int diff = cellIndex2 - cellIndex1;
		if (diff == -cols) // north path
		{
			cells[cellIndex1] |= NORTH_PATH;
			cells[cellIndex2] |= VISITED | SOUTH_PATH;
			visitedCellCount++;
		}
		else if (diff == cols) // south path
		{
			cells[cellIndex1] |= SOUTH_PATH;
			cells[cellIndex2] |= VISITED | NORTH_PATH;
			visitedCellCount++;
		}
		else if (diff == -1) // west path
		{
			cells[cellIndex1] |= WEST_PATH;
			cells[cellIndex2] |= VISITED | EAST_PATH;
			visitedCellCount++;
		}
		else if (diff == 1) // east path
		{
			cells[cellIndex1] |= EAST_PATH;
			cells[cellIndex2] |= VISITED | WEST_PATH;
			visitedCellCount++;
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
	std::function<void(void)> generationAlgorithm;

	
	int rows;
	int cols;
	int cellSize;
	int cellCount;
	int pathfindingStartCell = 0;
	int pathfindingEndCell = 0;
	int visitedCellCount = 0;
	
	


};

