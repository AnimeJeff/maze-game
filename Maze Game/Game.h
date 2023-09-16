#pragma once
#include "Maze.h"
#include "SFML/Graphics.hpp"
#include "Button.h"
class Game
{	
private:
	int cellSize             = 20;
	int cellWallThickness    = 1;
	sf::RectangleShape verticalWall   { sf::Vector2f(cellWallThickness * 2, cellSize) };
	sf::RectangleShape horizontalWall { sf::Vector2f(cellSize, cellWallThickness * 2) };
	sf::RectangleShape cellSquare     { sf::Vector2f(cellSize, cellSize) };

	Button button;

	int rows                 = 40;
	int cols                 = 50;
	int windowWidth          = cols * cellSize ;
	int windowHeight = rows * cellSize;// +200;
	sf::RenderWindow window;
	bool initialised = false;
	
	sf::Color pathfindingStartCellColour = sf::Color::Green;
	sf::Color pathfindingEndCellColour = sf::Color::Red;
	sf::RenderTexture mazeTex;
public:
	void init();
	void draw();
	sf::Vector2i getWindowMousePos() {
		return sf::Mouse::getPosition(window);
	}
	sf::Vector2f getGlobalMousePos() {
		return window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	void mouse();
	void keyboard();
	Maze maze{ rows,cols,cellSize };
};

