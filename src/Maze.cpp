#include "Maze.h"
#include <ctime>



void Cell::insert_neigbhor(Cell * c)
{
    Cell dc = *c + -(*this);
    
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
    
    open_neigbhors_[pos] = c;
}

Cell * Cell::rand_neigbhors(int dr, int dc)
{
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
        return open_neigbhors_[j];
    }

    pos = rand() % 4;
    while (open_neigbhors_[pos] == NULL || pos == opp_i)
    {
        pos = rand() % 4;
    }
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

void Maze::draw_maze(float r, float c, int dr, int dc)
{
    if (mode)
    {
        int x = (r+5) / scalex_;
        int y = (c+5) / scalez_;
        // std::cout << x << ' ' << y << std::endl;
        Cell * node = Map[x][y];
        std::stack< Wall > draw_walls;
    
        view_recur(node, 0, 0, draw_walls, 0);
        //view_recur(node, 0, dc, draw_walls, 0);
        // view_recur(node, -1, 0, draw_walls, 0);
        // view_recur(node, 0, 1, draw_walls, 0);
        //  view_recur(node, 0, -1, draw_walls, 0);
    
        glPushMatrix(); 
        {
            glScalef(scalex_, scaley_, scalez_);
            // std::cout << "starting\n";
            while (!draw_walls.empty())
            {
                Wall wall = draw_walls.top();
                // std::cout << wall << std::endl;
                wall.draw_wall();
                draw_walls.pop();
            }
        }
        glPopMatrix();
    }
    else
    {
    
        glPushMatrix();
        {
            glScalef(scalex_, scaley_, scalez_);
            // std::cout << "starting\n";

            for (int i = 0; i < walls.size(); ++i)
            {
                walls[i].draw_wall();
            }
        }
        glPopMatrix();
    
    }
    
}

void view_recur(Cell * node, int dr, int dc,
                std::stack< Wall > & walls, int flag)
{
    for (int i = 0; i < 4; ++i)
    {
        Cell c = Cell::diff(i);
        
        if (c + Cell(dr, dc) == Cell(0, 0))
            continue;
        
        Cell * next_node = node->neigbhor(i);
        
        if (next_node == NULL)
        {
            Cell next_cell = *node + c;
            walls.push(Wall(*node, next_cell, .2, 3, 1));
        }
        else
        {
            int flag_ = dr * c.c_  - dc * c.r_;
            if (flag_ != flag || flag_ == 0)
            {
                if (flag_ == 0)
                    flag_ = flag;
                view_recur(next_node, c.r_, c.c_, walls, flag_);
                // if ()
            }
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
