#include "Game.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
void Game::init() {
    verticalWall.setFillColor(sf::Color::Black);
    horizontalWall.setFillColor(sf::Color::Black);
    button.setFillColor(sf::Color::Magenta);
    button.setSize({ 200,200 });
    button.setPosition(sf::Vector2f{100.0f,(float) rows*cellSize-100});

    maze.setGenerationAlgorithm(Maze::DFS);

	window.create(sf::VideoMode(windowWidth, windowHeight), maze.getCurrentGenerationAlgorithm());
	window.setFramerateLimit(120);
    window.clear(sf::Color::White);
    draw();
    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                keyboard();
                break;
            case sf::Event::MouseButtonPressed:
                mouse();
                break;
            }
            
        }
    }
    initialised = true;
}

void Game::draw()
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {

            cellSquare.setPosition(col * cellSize, row * cellSize);
            //std::cout << "draw row: " << row << " col: " << col << "\n" ;
            int cellIndex = maze.getCellIndex(row, col);
            int cell = maze.cellAt(cellIndex);
            if (maze.finishedGenerating()) // All of the cells has been visited - maze is generated.
            { 
                if (maze.isPathfindingStartCell(cellIndex))
                {
                    cellSquare.setFillColor(pathfindingStartCellColour);
                }
                else if (maze.isPathfindingEndCell(cellIndex))
                {
                    cellSquare.setFillColor(pathfindingEndCellColour);
                }
                else if (maze.isPathfindingPathCell(cellIndex))
                {
                    cellSquare.setFillColor(sf::Color::Magenta);
                }
                else
                {
                    cellSquare.setFillColor(sf::Color::White);
                }
            }
            else
            {
                if (cellIndex == maze.getCurrentGenerationCell())
                {
                    cellSquare.setFillColor(sf::Color::Cyan);
                }
                else if ((cell & maze.VISITED_TWICE) == maze.VISITED_TWICE)
                {
                    cellSquare.setFillColor(sf::Color::Green);
                }
                else if ((cell & maze.VISITED) == maze.VISITED)
                {
                    cellSquare.setFillColor(sf::Color(255, 255, 208));
                }
                else
                {
                    cellSquare.setFillColor(sf::Color::Black);
                }
            }
            window.draw(cellSquare);

            // Drawing the walls for each cell
            if ((cell & maze.EAST_PATH) != maze.EAST_PATH)
            {
                int xPos = (col + 1) * cellSize - cellWallThickness;
                int yPos = row * cellSize;
                verticalWall.setPosition(xPos, yPos);
                window.draw(verticalWall);
            }
            if ((cell & maze.SOUTH_PATH) != maze.SOUTH_PATH)
            {
                int xPos = col * cellSize;
                int yPos = (row + 1) * cellSize - cellWallThickness;
                horizontalWall.setPosition(xPos, yPos);
                window.draw(horizontalWall);
            }
        }
    }

    //window.draw(button);
}



void Game::mouse() {

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i rowCol = maze.getRowColFromMousePos(sf::Mouse::getPosition(window));
        int cellIndex = maze.getCellIndexFromMousePos(sf::Mouse::getPosition(window));
        std::cout << "row: " << rowCol.x << '\t' << "col:" << rowCol.y << '\t' << "cell index: " << cellIndex << std::endl;
        button.checkClick(getGlobalMousePos());
        if (maze.finishedGenerating() &&
            (!maze.hasPathfindingStartCell() || !maze.hasPathfindingEndCell()))
        {
            if (!maze.hasPathfindingStartCell())
            {
                maze.setPathfindingStartCell(cellIndex);
            }
            else if (!maze.hasPathfindingEndCell() && !maze.isPathfindingStartCell(cellIndex))
            {
                maze.setPathfindingEndCell(cellIndex);
            }
            draw();
            window.display();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) 
    {
        int cellIndex = maze.getCellIndexFromMousePos(sf::Mouse::getPosition(window));
        maze.setSearchStart(cellIndex);
        draw();
        window.display();
        std::cout << "right\n";
    }
}

void Game::keyboard() {
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
    {
        window.close();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        maze.A_Star();
        for (auto cell : maze.path)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { 
               break; window.close();
            }
            maze.cells[cell] |= maze.PATH;
            window.clear(sf::Color::White);
            draw();
            window.display();

        }
        
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        maze.setGenerationAlgorithm(Maze::RECURSIVE_BACKTRACKING);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        maze.setGenerationAlgorithm(Maze::DFS);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
    {
        maze.setGenerationAlgorithm(Maze::PRIMS);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
    {
        maze.setGenerationAlgorithm(Maze::HUNT_AND_KILL);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        maze.setGenerationAlgorithm(Maze::KRUSKALS);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
    {
        maze.setGenerationAlgorithm(Maze::ALDOUS_BRODER);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
    {
        maze.setGenerationAlgorithm(Maze::BINARY_TREE);
        window.setTitle(maze.getCurrentGenerationAlgorithm());
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        window.clear(sf::Color::White);
        maze.generate();
        draw();
        window.display();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        static int dt;
        static int speed = 120;
        static auto microsecondsPerFrame = sf::microseconds(1000000 / speed).asMicroseconds();
        bool limitFPS = false;
        do {
            sf::Clock c;
            c.restart();
            maze.generate();
            window.clear(sf::Color::White);
            draw();
            window.display();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { break; }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { break; window.close(); }
            dt = microsecondsPerFrame - c.getElapsedTime().asMicroseconds();
            std::this_thread::sleep_for(std::chrono::microseconds(dt));
        } while (!maze.finishedGenerating());
        
        window.clear(sf::Color::White);
        draw();
        window.display();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        maze.reset();
        window.clear(sf::Color::White);
        while (!maze.finishedGenerating())
        {
            maze.generate();
        }
        draw();
        window.display();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        maze.reset();
        window.clear(sf::Color::White);
        draw();
        window.display();
    }
}

