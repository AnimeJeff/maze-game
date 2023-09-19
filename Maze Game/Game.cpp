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
    
	window.create(sf::VideoMode(windowWidth, windowHeight), maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            cellSquare.setPosition(col * cellSize, row * cellSize);
            int cellIndex = maze.getCellIndex(row, col);
        
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
                else if (maze.checkCellState(cellIndex, Maze::VISITED_TWICE))
                {
                    cellSquare.setFillColor(sf::Color::Green);
                }
                else if (maze.checkCellState(cellIndex, Maze::VISITED))
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
            if (!maze.checkCellState(cellIndex, Maze::EAST_PATH))
            {
                float xPos = (col + 1) * cellSize - cellWallThickness;
                float yPos = row * cellSize;
                verticalWall.setPosition(xPos, yPos);
                window.draw(verticalWall);
            }
            if (!maze.checkCellState(cellIndex, Maze::SOUTH_PATH))
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
        //std::cout << "row: " << rowCol.x << '\t' << "col:" << rowCol.y << '\t' << "cell index: " << cellIndex << std::endl;
        button.checkClick(getGlobalMousePos());
        if (maze.finishedGenerating())
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
                return;
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
    }
}

void Game::keyboard() {
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
    {
        window.close();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        for (auto cellIndex : maze.getPath())
        {
            maze.unsetCellState(cellIndex, Maze::PATH);
        }
        maze.find();
        for (auto cellIndex : maze.getPath())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                break; window.close();
            }
            maze.setCellState(cellIndex, Maze::PATH);
            window.clear(sf::Color::White);
            draw();
            window.display();
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        for (auto cellIndex : maze.getPath())
        {
            maze.unsetCellState(cellIndex, Maze::PATH);
        }
        maze.find();
        for (auto cellIndex : maze.getPath())
        {
            maze.setCellState(cellIndex, Maze::PATH);
        }
        window.clear(sf::Color::White);
        draw();
        window.display();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::RECURSIVE_BACKTRACKING);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::DFS);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::PRIMS);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::HUNT_AND_KILL);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::KRUSKALS);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::ALDOUS_BRODER);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
    {
        maze.setGenerationAlgorithm(GenerationAlgorithm::BINARY_TREE);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
    {
        maze.setPathfindingAlgorithm(PathfindingAlgorithm::ASTAR);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
    {
        maze.setPathfindingAlgorithm(PathfindingAlgorithm::BFS);
        window.setTitle(maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) break;
            
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

