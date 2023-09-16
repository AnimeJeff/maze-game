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
#include <unordered_set>
class Maze
{
private:
	class Algorithm 
	{
		
	public:
		Algorithm(Maze* maze, int startCell) :maze(maze), startCell(startCell) {}
		virtual std::string name() = 0;
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
		Maze* maze;
		int startCell;
		bool generationFinished = false;
		int currentCell = 0;
		
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
		void generate() override
		{
			if (visitedCells == maze->cellCount)
			{
				generationFinished = true;
				return;
			}
			std::vector<int> neighbours = maze->getNeighbours(q.back());
			while (neighbours.empty())
			{
				q.pop_back();
				maze->cells[q.back()] |= CellState::VISITED_TWICE;
				neighbours = maze->getNeighbours(q.back());
			}
			int nextCellIndex = maze->random(neighbours);
			maze->createPath(q.back(), nextCellIndex);
			visitedCells++;
			currentCell = nextCellIndex;
			q.push_back(nextCellIndex);
		}
		void reset() override
		{
			visitedCells = 0;
			generationFinished = false;
			q.clear();
			q.push_back(startCell);
			currentCell = startCell;
			maze->cells[startCell] |= VISITED;
			visitedCells++;
		}
	private:
		std::deque<int> q;
		int visitedCells = 0;
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
				generationFinished = true;
				return;
			}
			int frontier;
			std::vector<int> neighbours;
			int rnd = maze->random(frontiers.size());
			// getting a random frontier
			std::set<int>::iterator it = frontiers.begin();
			std::advance(it, rnd);
			frontier = *it;
			currentCell = frontier;
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

		}
		void reset() override
		{
			generationFinished = false;
			frontiers.clear();
			for (const auto& frontier : maze->getNeighbours(startCell))
			{
				frontiers.insert(frontier);
			}
			maze->cells[startCell] |= VISITED;
			currentCell = startCell;
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
			if (stack.empty()) {
				generationFinished = true;
				return;
			}
			auto neighbours = maze->getNeighbours(stack.top());
			if (neighbours.empty())
			{
				stack.pop();
				return;
			}
			currentCell = stack.top();
			int neighbour = maze->random(neighbours);
			maze->createPath(currentCell, neighbour);
			stack.push(neighbour);
		}
		void reset() override
		{
			currentCell = startCell;
			generationFinished = false;
			stack = std::stack<int>();
			stack.push(startCell);
		}
	private:
		std::stack<int> stack;
	};

	class HuntAndKill: public Algorithm
	{
	public:
		HuntAndKill(Maze* maze, int startCell) :Algorithm(maze, startCell)
		{
			currentCell = startCell;
			setStartCell(startCell);
		}
		std::string name() override
		{
			return "Hunt and Kill!";
		};
		void generate() override
		{
			
			if (visitedCells == maze->cellCount) // should be when currentcell is invalid
			{
				generationFinished = true;
				return;
			}
			auto neighbours = maze->getNeighbours(currentCell);
			// if there are neighbours
			if (!neighbours.empty())
			{
				auto neighbour = neighbours[maze->random(neighbours.size())];
				auto rndNeighbour = neighbours[maze->random(neighbours.size())];
				maze->createPath(currentCell, rndNeighbour);
				visitedCells++;
				currentCell = rndNeighbour;
			}
			// if no neighbours
			else
			{
				// hunting for an unvisited cell with a visited neighbour
				for (int i = firstUnvisitedCell; i < maze->cellCount; i++)
				{
					bool visited = (maze->cellAt(i) & VISITED) == VISITED;
					if (!visited)
					{
						// checking if any of its neighbours have been visited
						auto visitedNeighbours = maze->getNeighbours(i, true);
						if (!visitedNeighbours.empty())
						{
							firstUnvisitedCell = i + 1;
							auto rndNeighbour = visitedNeighbours[maze->random(visitedNeighbours.size())];
							maze->createPath(rndNeighbour, i);
							visitedCells++;
							currentCell = i;
							break;
						}
					}
				}
			}
		};
		void reset() override 
		{
			visitedCells = 0;
			generationFinished = false;
			firstUnvisitedCell = 0;
			currentCell = startCell;
			maze->cells[startCell] |= VISITED;
			visitedCells++;
		};
	private:
		
		int firstUnvisitedCell = 0;
		int visitedCells = 0;
	};

	class Kruskals : public Algorithm
	{
	public:
		Kruskals(Maze* maze, int startCell) :Algorithm(maze, startCell)
		{
			setStartCell(startCell);
		}
		std::string name() override
		{
			return "Kruskal's Algorithm";
		}
		void generate() override
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
					maze->cells[cell.first] |= VISITED;
					maze->visitedCellCount++;
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
					map[*it]= map[cell.first];
					map[cell.first]->insert(*it);
					it++;
				}
			}
			
		}
		void reset() override
		{
			generationFinished = false;
			currentCell = startCell;
			edges.clear();
			for (int i = 0; i < maze->cellCount; i++)
			{
				
				int col = i % maze->cols;
				if (i >= maze->cols) {
					static int rows = 0;
					rows++;
					edges.push_back({ i , i - maze->cols });
					//std::cout << "rows " << rows << "\n";
				}
				if (col > 0)
				{
					static int cols = 0;
					edges.push_back({ i , i - 1 });
					cols++;
					//std::cout << "cols " << cols << "\n";
				}
				map.clear();
			}
			std::shuffle(std::begin(edges), std::end(edges), maze->mt);
		}
		std::unordered_map<int, std::shared_ptr<std::set<int>>> map; //
		std::vector<std::pair<int,int>> edges;
		int currentSetCount = 0;
	private:
		
	};

	friend Dfs;
	friend Prims;
	int generationStartCell;
	std::shared_ptr<Algorithm> generationAlgo = nullptr;

	int random(int max = 10000) {
		assert(max <= 10000);
		return intdist(mt) % max;
	}

	int random(std::vector<int>& list) {
		int out;
		std::sample(list.begin(), list.end(), &out, 1, mt);
		return out;
	}

public:
	enum AlgorithmType {
		RECURSIVE_BACKTRACKING,
		DFS,
		PRIMS,
		HUNT_AND_KILL,
		KRUSKALS
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
		case KRUSKALS:
			generationAlgo = std::make_unique<Kruskals>(this, generationStartCell);
			break;
		case PRIMS:
			generationAlgo = std::make_shared<Prims>(this, generationStartCell);
			break;
		case HUNT_AND_KILL:
			generationAlgo = std::make_shared<HuntAndKill>(this, generationStartCell);
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
	int getCurrentGenerationCell() {
		return generationAlgo->getCurrentCell();
	}
public:
	inline void generate() {
		generationAlgo->generate();
	}
	inline bool finishedGenerating()
	{
		return generationAlgo->finishedGenerating();
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
	inline void setSearchStart(int row, int col) 
	{
		setSearchStart(getCellIndex(row, col));
	}
	void setSearchStart(int cellIndex) // sets the starting cell for the generation algorithm
	{
		generationStartCell = cellIndex;
		if (generationAlgo)
		{
			generationAlgo->setStartCell(generationStartCell);
		}
		reset();
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
			cells[cellIndex1] |= VISITED | NORTH_PATH;
			cells[cellIndex2] |= VISITED | SOUTH_PATH;
			visitedCellCount++;
		}
		else if (diff == cols) // south path
		{
			cells[cellIndex1] |= VISITED | SOUTH_PATH;
			cells[cellIndex2] |= VISITED | NORTH_PATH;
			visitedCellCount++;
		}
		else if (diff == -1) // west path
		{
			cells[cellIndex1] |= VISITED | WEST_PATH;
			cells[cellIndex2] |= VISITED | EAST_PATH;
			visitedCellCount++;
		}
		else if (diff == 1) // east path
		{
			cells[cellIndex1] |= VISITED | EAST_PATH;
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

