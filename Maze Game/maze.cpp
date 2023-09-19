#include "Maze.h"


void Maze::reset()
{
    std::fill(cells.begin(), cells.end(), 0);
    pathfindingStartCell = -1;
    pathfindingEndCell = -1;
    //path.clear();
    if (generator)
    {
        generator.get()->reset();
    }
    if (pathfinder)
    {
        pathfinder->setStart(-1);
        pathfinder->setEnd(-1);
        pathfinder.get()->reset();
    }
}

std::vector<int> Maze::getNeighbours(int cellIndex, bool visited)
{
    std::vector<int> neighbours;
    int northCellIndex = cellIndex - cols;
    int southCellIndex = cellIndex + cols;
    int westCellIndex = cellIndex - 1;
    int eastCellIndex = cellIndex + 1;

    int cellVisited = visited ? VISITED : 0;

    if (northCellIndex >= 0 &&
        ((cells[northCellIndex] & VISITED) == cellVisited))
    {
        neighbours.push_back(northCellIndex);
    }

    if (southCellIndex < cells.size() &&
        ((cells[southCellIndex] & VISITED) == cellVisited))
    {
        neighbours.push_back(southCellIndex);
    }

    if (cellIndex % cols != 0 && westCellIndex >= 0 &&
        ((cells[westCellIndex] & VISITED) == cellVisited))
    {
        neighbours.push_back(westCellIndex);
    }

    if (cellIndex % cols != cols - 1 && eastCellIndex < cells.size() &&
        ((cells[eastCellIndex] & VISITED) == cellVisited))
    {
        neighbours.push_back(eastCellIndex);
    }

    return neighbours;
}

void Maze::print_maze()
{

    for (int i{};i < rows;++i) {
        for (int j=i*cols;j < (i+1) * cols;++j) {

            if ((cells[j] & VISITED) == VISITED) {
                std::cout << " " << 1 << " ";
            }
            else
            {
                std::cout << " " << 0 << " ";
            }

            
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


