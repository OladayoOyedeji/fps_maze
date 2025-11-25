#include "Maze.h"
#include <ctime>

Cell d[] = {Cell(-1, 0), Cell(1, 0), Cell(0, -1), Cell(0, 1)};

bool valid_move(const Cell & cell, int n)
{
    return (cell.r_ < n && cell.r_ >= 0 && cell.c_ < n && cell.c_ >= 0);
}

std::vector< Wall > build_maze(int n, int r, int c)
{
    srand((unsigned int) time(NULL));

    std::set<Cell> VISITED;
    VISITED.insert(Cell(r, c));
    
    std::stack<Cell> PATH;
    PATH.push(Cell(r, c));
    
    std::vector<Wall> PUNCH;
    bool visited;
    Cell cell = PATH.top();

    while (!(PATH.empty()))
    {
        // 0:up, 1:down, 2:left, 3:rght
        int move = rand() % 4;
        int i = move;
        //
        Cell new_cell = cell + d[i];

        std::cout << " (" << cell.r_ << ", " << cell.c_ << ")" << std::endl;
        while (!valid_move(new_cell, n) || VISITED.find(new_cell) != VISITED.end())
        {
            std::cout << i << " (" << new_cell.r_ << ", " << new_cell.c_ << ")" << std::endl;
            i = (i+3)%4;
            if (i == move) break;
            new_cell = cell + d[i];  
        }
        if (!valid_move(new_cell, n) || VISITED.find(new_cell) != VISITED.end())
        {
            PATH.pop();
            if (!PATH.empty())
                cell = PATH.top();
        }
        else
        {

            VISITED.insert(new_cell);
            PUNCH.push_back(Wall(cell, new_cell));
            cell = new_cell;
            PATH.push(cell);
        }
    }
    std::string delim = "";
    std::cout << "{";
    for (int i = 0; i < PUNCH.size(); ++i)
    {
        std::cout << delim << "((" << PUNCH[i].c0.r_ << ',' << PUNCH[i].c0.c_ << "),("
                  << PUNCH[i].c1.r_ << ',' << PUNCH[i].c1.c_ << "))";
        delim = ", ";
    }
    std::cout << "}\n";
    return PUNCH;
}


void print_maze(int n, const std::vector<Wall> & p)
{
    std::cout << '+';
    for (int i = 0; i < n; ++i)
    {
        std::cout << "-+";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; ++i)
    {
        std::cout << '|';
        for (int j = 0; j < n; ++j)
        {
            bool work = false;
            if ( j != n - 1 )
            {
                Wall cell(i, j, i, j + 1);
                for (int k = 0; k < p.size(); ++k)
                {
                    if (cell == p[k])
                    {
                        work = true;
                        break;
                    }
                }
            }
            if (work)
            {std::cout << "  ";}
            else
                std::cout << " |";
        }
        std::cout << std::endl
                  << '+';
        
        for (int j = 0; j < n; ++j)
        {
            bool work = false;
            if ( i != n - 1 )
            {
                Wall cell(i, j, i + 1, j);
                for (int k = 0; k < p.size(); ++k)
                {
                    if (cell == p[k])
                    {
                        work = true;
                        break;
                    }
                }
            }
            if (work)
            {
                std::cout << " +";
            }
            else
                std::cout << "-+";
            
        }
        std::cout << std::endl;
    }
}
