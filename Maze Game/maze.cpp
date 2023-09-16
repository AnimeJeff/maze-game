#include "Maze.h"



void Maze::Bfs()
{
    //if ((cells[q.front()] & VISITED) == 0) // Not Visited
    //{
    //    cells[q.front()] |= VISITED;
    //    visitedCellCount++;
    //    auto neighbours = getNeighbours(q.front());
    //    std::shuffle(std::begin(neighbours), std::end(neighbours), mt);
    //    for (const auto& neighbour : neighbours) {
    //        q.push_back(neighbour);
    //    }
    //}
    //q.pop_front();
}

void Maze::reset()
{
    std::fill(cells.begin(), cells.end(), 0);
    visitedCellCount = 0;
    generationAlgo.get()->reset();

    pathfindingStartCell = 0;
    pathfindingEndCell = 0;
    path.clear();
}



void Maze::A_Star()
{
    //return;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> priority_q;
    std::map<int, int> gn{};
    for (int i{};i < rows * cols;i++) { gn[i] = 9999; };
    std::map<int, int> fn{};
    for (int i{};i < rows * cols;i++) { fn[i] = 9999; };
    std::map<int, int> came_from{};
    std::set<int> closed;
    
    auto get_hn = [&](int cellIndex) {
        return int((pathfindingEndCell - cellIndex) / cols) + int((pathfindingEndCell - cellIndex) % cols);
    };
    auto get_neighbours = [&](int cellIndex) {
        std::vector<int>neighbours;

        if (((cells[cellIndex] & NORTH_PATH) == NORTH_PATH) && !closed.contains(cellIndex - cols)) 
        { 
            neighbours.push_back(cellIndex - cols); 
        }
        if (((cells[cellIndex] & SOUTH_PATH) == SOUTH_PATH) && !closed.contains(cellIndex + cols)) 
        {
            neighbours.push_back(cellIndex + cols);
        }
        if (((cells[cellIndex] & WEST_PATH) == WEST_PATH) && !closed.contains(cellIndex - 1)) 
        {
            neighbours.push_back(cellIndex - 1);
        }
        if (((cells[cellIndex] & EAST_PATH) == EAST_PATH) && !closed.contains(cellIndex + 1)) 
        {neighbours.push_back(cellIndex + 1);
        }
        return neighbours;
    };
    
    std::cout << "start: " << pathfindingStartCell << std::endl;
    std::cout << "end: " << pathfindingEndCell << std::endl;

    fn[pathfindingStartCell] = get_hn(pathfindingStartCell);
    gn[pathfindingStartCell] = 0;
    came_from[pathfindingStartCell] = pathfindingStartCell;
    priority_q.push(std::make_pair(fn[pathfindingStartCell], pathfindingStartCell));
    closed.insert(pathfindingStartCell);

    while (!(priority_q.empty()))
    {
        auto current = priority_q.top();
        if (current.second == pathfindingEndCell) 
        { 
            std::cout << current.second << " found" <<std::endl; break; 
        }
        auto cost_so_far = gn[current.second];
        auto neigbours = get_neighbours(current.second);
        priority_q.pop();
        for (auto neighbour : neigbours)
        {
            if (cost_so_far + 1 < gn[neighbour])
            {
                gn[neighbour] = cost_so_far + 1;
                fn[neighbour] = gn[neighbour] + get_hn(neighbour);
                came_from[neighbour] = current.second;
                
                if (!(closed.contains(neighbour)))
                {
                    priority_q.push(std::make_pair(fn[neighbour], neighbour));
                    closed.insert(neighbour);
                }
            }
        }
    }
    auto construct_path = [&]() {
        int current = came_from[pathfindingEndCell];
        //maze[current] |= PATH;
        path.push_back(current);
        while (came_from[current] != pathfindingStartCell) {
            current = came_from[current];
            path.insert(path.begin(),current);
            //maze[current] |= PATH;
        }
    };
    construct_path();
    
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


