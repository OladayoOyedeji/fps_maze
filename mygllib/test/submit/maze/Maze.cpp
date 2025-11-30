#include "Maze.h"
#include <ctime>

std::vector< PunchedWall > build_maze(int n, int r, int c)
{
    srand((unsigned int) time(NULL));
    DLList<Cell> VISITED;
    VISITED.push_front(Cell(r, c));
    Stack<Cell> PATH;
    PATH.push(Cell(r, c));
    std::vector<PunchedWall> PUNCH;
    bool visited;
    Cell x = PATH.top();
    while (!(PATH.empty()))
    {
        Cell y(x);
        bool visited = true;
        std::vector< Cell > moves;
        if (x.c_ + 1 < n)
        {
            moves.push_back(Cell(x.r_, x.c_ + 1));
        }
        if (x.c_ - 1 >= 0)
        {
            moves.push_back(Cell(x.r_, x.c_ - 1));
        }
        if (x.r_ + 1 < n)
        {
            moves.push_back(Cell(x.r_ + 1, x.c_));
        }
        if (x.r_ - 1 >= 0)
        {
            moves.push_back(Cell(x.r_ - 1, x.c_));
        }
        // if (moves.size() != 0)
            
        // int move = rand() % moves.size();
        int i = 0;
        while (moves.size() != 0 && visited)
        {
            int move = rand() % moves.size();
            visited = false;
            
            for (DLList< Cell >::iterator p = VISITED.begin();
                 p != VISITED.end(); ++p)
            {
                if (moves[move] == *p)
                {
                    visited = true;
                    moves.erase(std::vector<Cell>::iterator(&moves[move]));
                    break;
                }
            }
            if (!visited)
                y = moves[move];
        }
        if (!visited)
        {
            VISITED.push_back(y);
            PATH.push(y);
            PUNCH.push_back(PunchedWall(x, y));
            x = y;
        }
        else
        {
            PATH.pop();
            if (!PATH.empty())
                x = PATH.top();
        }
        //==========================================================
        // now baby .... why dont you just meet me in the middle ...
        // i am loosing my mind just a little ... so why dont you
        // just meet me in the middle baby
        //==========================================================

    }
    return PUNCH;
}

void print_maze(int n, const std::vector<PunchedWall> & p)
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
                PunchedWall cell(i, j, i, j + 1);
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
                PunchedWall cell(i, j, i + 1, j);
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
