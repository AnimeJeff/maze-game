#pragma once
#include "Maze.h"
#include "SFML/Graphics.hpp"
#include <chrono>
#include <thread>
#include <memory>
class Game
{	
private:
	unsigned int cellSize             = 20;
	unsigned int cellWallThickness    = 1;
	sf::RectangleShape verticalWall   { sf::Vector2f(cellWallThickness * 2, cellSize) };
	sf::RectangleShape horizontalWall { sf::Vector2f(cellSize, cellWallThickness * 2) };
	sf::RectangleShape cellSquare     { sf::Vector2f(cellSize, cellSize) };

	unsigned int rows                 = 40;
	unsigned int cols                 = 50;
	int windowWidth					  = cols * cellSize ;
	int windowHeight = rows * cellSize;// +200;
	sf::RenderWindow window;
	bool initialised = false;
	
	sf::Color pathfindingStartCellColour = sf::Color::Green;
	sf::Color pathfindingEndCellColour = sf::Color::Red;
	bool shouldGenerateDraw = false;
	int mazeGenerationFPS = 20;
	sf::Int64 microsecondsPerFrame = sf::microseconds(1000000 / mazeGenerationFPS).asMicroseconds();
	void setFPS(int FPS)
	{
		mazeGenerationFPS = FPS;
		microsecondsPerFrame = sf::microseconds(1000000 / FPS).asMicroseconds();
	}
	bool limitFPS = false;
	bool stopGenerating = false;
	bool trySetPathfindingCells(int cellIndex)
	{
		if (!maze.hasPathfindingStartCell() && !maze.isPathfindingEndCell(cellIndex))
		{
			maze.setPathfindingStartCell(cellIndex);
		}
		else if (maze.isPathfindingStartCell(cellIndex))
		{
			maze.unsetPathfindingStartCell();
		}
		else if (!maze.hasPathfindingEndCell() && !maze.isPathfindingStartCell(cellIndex))
		{
			maze.setPathfindingEndCell(cellIndex);
		}
		else if (maze.isPathfindingEndCell(cellIndex))
		{
			maze.unsetPathfindingEndCell();
		}
		else
		{
			return false;
		}
		return true;
	}
	
	void draw();
	void visualiseGeneration(); // draws how the maze is generated step by step
	void visualisePathfinding();
	void setGenerationAlgorithm(GenerationAlgorithm::Type algorithm)
	{
		maze.setGenerationAlgorithm(algorithm);
		window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
		draw();
	}
	void setPathfindingAlgorithm(PathfindingAlgorithm::Type algorithm)
	{
		maze.setPathfindingAlgorithm(algorithm);
		window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
		draw();
	}
	void handleMouseEvent();
	void handleKeyEvent(sf::Event::KeyEvent keyEvent);
	Maze maze{ rows,cols,cellSize };
public:
	void init();
	
};

