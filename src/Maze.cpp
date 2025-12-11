#include "Maze.h"
#include <ctime>



void Cell::insert_neigbhor(Cell * c)
{
    Cell dc = *c + -(*this);
    std::cout << *c << ' ' << *this << ' ' << dc << std::endl;
        
    int pos = 0;
    switch (dc.c_)
    {
        case 0:
            switch (dc.r_)
            {
                case 1:
                    pos = S_;
                    break;
                case -1:
                    pos = N_;
                    break;
            }
            break;
        case 1:
            pos = E_;
            break;
        case -1:
            pos = W_;
    }
    std::cout << pos << std::endl;
    open_neigbhors_[pos] = c;
}

Cell * Cell::rand_neigbhors(int dr, int dc)
{
    std::cout << *this << ' ' << Cell(dr, dc) << std::endl;
    int pos = 0;
    int opp_i;
    switch (dc)
    {
        case 0:
            switch (dr)
            {
                case 1:
                    pos = S_;
                    opp_i = N_;
                    break;
                case -1:
                    pos = N_;
                    opp_i = S_;
                    break;
            }
            break;
        case 1:
            pos = E_;
            opp_i = W_;
            break;
        case -1:
            pos = W_;
            opp_i = E_;
    }
    std::cout << pos << std::endl;
    int size = 0;
    int j = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (open_neigbhors_[i] != NULL)
        {
            size++;
            j = i;
        }
    }

    if (size == 1)
    {
        std::cout << "opposite " << j << std::endl;;
        std::cout << *open_neigbhors_[j] << std::endl;
        return open_neigbhors_[j];
    }

    pos = rand() % 4;
    while (open_neigbhors_[pos] == NULL || pos == opp_i)
    {
        pos = rand() % 4;
    }
        
    std::cout << "left or right or front " << pos << std::endl;
    std::cout << *open_neigbhors_[j] << std::endl;
    return open_neigbhors_[pos];
            
}

bool valid_move(const Cell & cell, int n)
{
    // std::cout << "Cell: " << cell << std::endl; 
    return (cell.r_ < n && cell.r_ >= 0 && cell.c_ < n && cell.c_ >= 0);
}

std::vector< Wall > build_maze(int n, int r, int c)
{
    
    Cell d[] = {Cell(-1, 0), Cell(1, 0), Cell(0, -1), Cell(0, 1)};

    srand((unsigned int) time(NULL));

    std::set<Cell> VISITED;
    VISITED.insert(Cell(r, c));
    
    std::stack<Cell> PATH;
    PATH.push(Cell(r, c));
    
    std::vector<Wall> PUNCH;
    
    Cell cell = PATH.top();
    std::cout << "help i am a maze and i am being built\n";
    while (!(PATH.empty()))
    {
        //std::cout << "path has a size of: " << PATH.size() << std::endl;
        // 0:up, 1:down, 2:left, 3:rght
        int move = rand() % 4;
        int i = move;
        //
        // std::cout << i << ' ' << d[i] << std::endl;
        Cell new_cell = cell + d[i];

        while (!valid_move(new_cell, n) || VISITED.find(new_cell) != VISITED.end())
        {
            i = (i+3)%4;
            if (i == move) break;
            new_cell = cell + d[i];  
        }
        // std::cout << "new_cell: " << new_cell << std::endl;
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

void Maze::draw_maze(int r, int c)
{
    Cell * cell = Map[r][c];
    std::stack< Wall > & walls;
    for (int i = 0; i < 4; ++i)
    {
        Cell * next_node = node->neigbhor(i);
        view_recur(next_node, dc_.r, dc_.c, walls);
    }
    glPushMatrix();
    {
        glScalef(scalex_, scaley_, scalez_);
        for (int i = 0; i < walls.size(); ++i)
        {
            // std::cout << "drawing wall " << walls[i].c0 << ' ' << walls[i].c1 << std::endl;;
            walls[i].draw_wall();
        }
    }
    glPopMatrix();
}

void view_recur(Cell * node, int dr, int dc, std::stack< Wall > & walls)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < 4; ++i)
    {
        Cell * next_node = node->neigbhor(i);
        if (next_node != NULL)
        {
            Cell dc_ = *node - *(next_node);
            
            if (dc_.r != dc || dc_.c != -dr)
            {
                walls.push(Wall(*node, *next_node));
                view_recur(next_node, dc_.r, dc_.c, walls);
            }
            // if ()
        }
    }
}

Maze::Maze(int n, float sx, float sy, float sz)
    : n_(n), scalex_(sx), scaley_(sy), scalez_(sz)
{
    Map = new Cell**[n];
    for (int i = 0; i < n; ++i )
    {
        Map[i] = new Cell*[n];
        for (int j = 0; j < n; j++)
        {
            Map[i][j] = new Cell(i, j);
        } 
    }
    std::cout << "Build maze!!!\n";
    std::vector< Wall > p_w = build_maze(n, 0, 0);
    print_maze(n_, p_w);
    for (int i = 0; i < p_w.size(); ++i)
    {
        // punched_walls.insert(p_w[i]);
         
        // Wall w = Wall(p_w[i].c1, p_w[i].c0);
         
        // punched_walls.insert(w);
         
        Map[p_w[i].c1.r_][p_w[i].c1.c_]->insert_neigbhor(Map[p_w[i].c0.r_][p_w[i].c0.c_]);
        Map[p_w[i].c0.r_][p_w[i].c0.c_]->insert_neigbhor(Map[p_w[i].c1.r_][p_w[i].c1.c_]);
    }

    for (int i = 0; i < n_; i++)
    {
        walls.push_back(Wall(Cell(i, 0), Cell(i,-1), .2, 3, 1));
        walls.push_back(Wall(Cell(0, i), Cell(-1,i), .2, 3, 1));
        for (int j = 0; j < n_; j++)
        {
            if (Map[i][j]->S() == NULL)
            {
                // std::cout << "{" << i << ' ' << j << "} wall here " << std::endl;
                walls.push_back(Wall(Cell(i, j), Cell(i+1, j), .2, 3, 1));
                    
            }
            if (Map[i][j]->E() == NULL)
            {
                // std::cout << "{" << i << ' ' << j << "} wall here " << std::endl;
                walls.push_back(Wall(Cell(i, j), Cell(i, j+1), .2, 3, 1));
            }
        }
        walls.push_back(Wall(Cell(i, n-1), Cell(i, n), .2, 3, 1));
        walls.push_back(Wall(Cell(n-1, i), Cell(n, i), .2, 3, 1));
    }
}
