#include "Game.h"

void Game::init() {
    verticalWall.setFillColor(sf::Color::Black);
    horizontalWall.setFillColor(sf::Color::Black);
    initialised = true;
	window.create(sf::VideoMode(windowWidth, windowHeight), maze.currentGenerationAlgorithmName() + " : " + maze.currentPathfindingAlgorithmName());
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(120);

    draw();
    sf::View view = window.getDefaultView();
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
                handleKeyEvent(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                handleMouseEvent();
                break;
            case sf::Event::Resized:
                rows = event.size.height / cellSize;
                cols = event.size.width / cellSize;
                unsigned int newWidth = cols * cellSize;
                unsigned int newHeight = rows * cellSize;
                window.setSize({ newWidth, newHeight });
                window.setView(sf::View(sf::FloatRect(0, 0, newWidth, newHeight)));
                maze.setDimensions(rows, cols);
                draw();
                break;
            }
            
            
        }
    }
}

void Game::draw()
{
    window.clear(sf::Color::Black);
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
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
    window.display();

}

void Game::visualiseGeneration() {
    int dt;
    sf::Clock c;
    stopGenerating = false;
    while (!maze.finishedGenerating())
    {
        c.restart();
        maze.generate();
        draw();
        if (limitFPS)
        {
            dt = microsecondsPerFrame - c.getElapsedTime().asMicroseconds();
            std::this_thread::sleep_for(std::chrono::microseconds(dt));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            return;
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            handleMouseEvent();
            break;
        }
  
    }
    draw();

}

void Game::visualisePathfinding()
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
        draw();
    }
}

void Game::handleMouseEvent()
{
    sf::Vector2i rowCol;
    int cellIndex;
    //std::cout << "row: " << rowCol.x << '\t' << "col:" << rowCol.y << '\t' << "cell index: " << cellIndex << std::endl
    try
    {
        cellIndex = maze.getCellIndexFromMousePos(sf::Mouse::getPosition(window));
        rowCol = maze.getRowColFromMousePos(sf::Mouse::getPosition(window));
    }
    catch (...)
    {
        return;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        
        if (maze.finishedGenerating() && trySetPathfindingCells(cellIndex))
        {
            draw();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) 
    {
        int cellIndex = maze.getCellIndexFromMousePos(sf::Mouse::getPosition(window));
        maze.setSearchStart(cellIndex);
        draw();
    }
}

void Game::handleKeyEvent(sf::Event::KeyEvent key)
{
    if ((key.control && key.code == sf::Keyboard::W) || key.code == sf::Keyboard::Escape)
    {
        window.close();
    }
    
    switch (key.code)
    {
    case sf::Keyboard::Num1:
        setGenerationAlgorithm(GenerationAlgorithm::ALDOUS_BRODER);
        break;
    case sf::Keyboard::Num2:
        setGenerationAlgorithm(GenerationAlgorithm::BINARY_TREE);
        break;
    case sf::Keyboard::Num3:
        setGenerationAlgorithm(GenerationAlgorithm::DFS);
        break;
    case sf::Keyboard::Num4:
        setGenerationAlgorithm(GenerationAlgorithm::ELLERS);
        break;
    case sf::Keyboard::Num5:
        setGenerationAlgorithm(GenerationAlgorithm::HUNT_AND_KILL);
        break;
    case sf::Keyboard::Num6:
        setGenerationAlgorithm(GenerationAlgorithm::KRUSKALS);
        break;
    case sf::Keyboard::Num7:
        setGenerationAlgorithm(GenerationAlgorithm::PRIMS);
        break;
    case sf::Keyboard::Num8:
        setGenerationAlgorithm(GenerationAlgorithm::RECURSIVE_BACKTRACKING);
        break;
    case sf::Keyboard::Num9:
        setGenerationAlgorithm(GenerationAlgorithm::WILSONS);
        break;
    case sf::Keyboard::Num0:
        setGenerationAlgorithm(GenerationAlgorithm::SIDEWINDER);
        break;
    case sf::Keyboard::Numpad1:
        setPathfindingAlgorithm(PathfindingAlgorithm::A_STAR);
        break;
    case sf::Keyboard::Numpad2:
        setPathfindingAlgorithm(PathfindingAlgorithm::BFS);
        break;
    case sf::Keyboard::Numpad3:
        setPathfindingAlgorithm(PathfindingAlgorithm::DEAD_END_FILLING);
        break;
    case sf::Keyboard::Numpad4:
        setPathfindingAlgorithm(PathfindingAlgorithm::WALL_FOLLOWER);
        break;
    case sf::Keyboard::D:
        maze.generate();
        draw();
        break;
    case sf::Keyboard::E:
        visualiseGeneration();
        break;
    case sf::Keyboard::G:
        maze.reset();
        while (!maze.finishedGenerating())
        {
            maze.generate();
        }
        draw();
        break;
    case sf::Keyboard::R:
        maze.reset();
        draw();
        break;
    case sf::Keyboard::F:
        if (!maze.finishedGenerating()) return;
        for (auto cellIndex : maze.getPath())
        {
            maze.unsetCellState(cellIndex, Maze::PATH);
        }
        maze.find();
        for (auto cellIndex : maze.getPath())
        {
            maze.setCellState(cellIndex, Maze::PATH);
        }
        draw();
        break;
    case sf::Keyboard::C:
        if (!maze.finishedGenerating()) return;
        for (auto cellIndex : maze.getPath())
        {
            maze.unsetCellState(cellIndex, Maze::PATH);
        }
        draw();
        break;
    case sf::Keyboard::S:
        if (!maze.finishedGenerating()) return;
        visualisePathfinding();
        break;
}
    
}


