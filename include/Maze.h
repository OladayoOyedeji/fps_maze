#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include "Utils.h"
#include "myglm.h"

enum DIRECTION
{
    N_, S_, W_, E_
};


class Cell
{
public:
    Cell(int r = 0, int c = 0)
        : r_(r), c_(c)
    {
        for (int i = 0; i < 4; ++i)
        {
            open_neigbhors_[i] = NULL;
        }
    }
    Cell(const Cell & c)
        : r_(c.r_), c_(c.c_)
    {}
    bool out_bound(int n)
    {
        return (c_ >= n || c_ < 0 || r_ >= n || r_ < 0);
    }
    Cell & operator=(const Cell & c)
    {
        r_ = c.r_;
        c_ = c.c_;
        return *this;
    }
    bool operator==(const Cell & c) const
    {
        return (r_ == c.r_ && c_ == c.c_);
    }
    Cell operator+(const Cell & c)
    {
        return Cell(r_ + c.r_, c_ + c.c_);
    }

    Cell operator-()
    {
        return Cell(-r_, -c_);
    }

    Cell operator-(const Cell & c)
    {
        return Cell(r_ - c.r_, c_ - c.c_);
    }

    bool operator<(const Cell& other) const {
        if (r_ != other.r_) return r_ < other.r_;
        return c_ < other.c_;
    }
    Cell * S()
    {
        return open_neigbhors_[S_];
    }

    Cell * E()
    {
        return open_neigbhors_[E_];
    }
    static Cell diff(int i)
    {
        switch (i)
        {
            case 0:
                return Cell(-1, 0);
            case 1:
                return Cell(1, 0);
            case 2:
                return Cell(0, -1);
            case 3:
                return Cell(0, 1);
        }
        return Cell(0, 0);
    }
    int orientation(Cell &dc)
    {
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
        return -1;
    }
    Cell * neigbhor(int i)
    {
        return open_neigbhors_[i];
    }
    void insert_neigbhor(Cell * c);
    Cell * rand_neigbhors(int dr, int dc);
    
    int r_, c_;

    Cell * open_neigbhors_[4];
    // std::vector<Cell *> open_neigbhors_;
};


inline
std::ostream & operator<<(std::ostream & cout, const Cell & w)
{
    std::cout << "(" << w.r_ << ", " << w.c_ << ")";
    return cout;
}

class Wall
{
public:
    Wall(const Cell cel, const Cell cell, float w=1, float h=1, float l=1)
        : c0(cel), c1(cell), h_(h)
    {
        
        Cell dc = c0 + -c1;
        switch (dc.r_)
        {
            case 0:
                w_ = l;
                l_ = w;
                break;
            default:
                w_ = w;
                l_ = l;
        }
    }
    Wall(const int r0_, const int c0_, const int r1_, const int c1_)
        : c0(r0_, c0_), c1(r1_, c1_)
    {
        // std::cout << c0 << ' ' << c1 << std::endl;; 
    }
    Wall(const Wall & w)
        : c0(w.c0), c1(w.c1), w_(w.w_), h_(w.h_), l_(w.l_)
    {
        //std::cout << c0 << ' ' << c1 << std::endl;;
    }
    Wall & operator=(const Wall & w)
    {
        w_ = w.w_;
        h_ = w.h_;
        l_ = w.l_;
        std::cout << "this? eq operator\n";
        c0 = w.c0; c1 = w.c1; return *this;
    }
    bool operator==(const Wall & p) const
    {
        return ((p.c0 == c0 && p.c1 == c1) || (p.c0 == c1 && p.c1 == c0));
    }
    bool operator<(const Wall & w) const
    {
        return (c0 < w.c0 && c1 < w.c1);
    }
    float x() const
    {
        return float(c0.r_ + c1.r_)/2;
    }
    
    float z() const
    {
        return float(c0.c_ + c1.c_)/2;
    }
    
    void draw_wall()
    {
        float c = float(c1.c_ + c0.c_) / 2;
        float r = float(c1.r_ + c0.r_) / 2;
        glPushMatrix();
        {
            glTranslatef(c1.r_, 1.5, c1.c_);
            Util::build_grass(1, h_, 1);
        }
        glPopMatrix();
        
        glPushMatrix();
        {
            glTranslatef(c0.r_, 1.5, c0.c_);
            Util::build_grass(1, h_, 1);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(r, 1.5, c);
            // std::cout << r << ' ' << c << std::endl;
            Util::build_wall(w_, h_, l_);
        }
        glPopMatrix();
    }
    Cell c0, c1;
    float w_, h_, l_;
};

inline
std::ostream & operator<<(std::ostream & cout, const Wall & w)
{
    std::cout << "[" << w.c0 << ',' << w.c1 << "]";
    return cout;
}

class Maze
{
public:
    Maze(int n=20, float sx=1, float sy=1, float sz=1);
    void draw_maze(float r = 0, float c = 0, int dr=1, int dc=0);
    float scalex_;
    float scaley_;
    float scalez_;
    int n_;
    bool mode;
    Cell *** Map;
    std::vector< Wall > walls;
};
// class MazeMap
// {
//     MazeMap(int n);
//     void print_maze();
//     void draw_maze()
//     {
        
//     }
    
//     std::unordered_set< Wall, WallHash > punched_walls;
// };

std::vector< Wall > build_maze(int n, int r, int c);
void print_maze(int n, const std::vector<Wall> & p);
void view_recur(Cell * node, int dr, int dc,
                std::stack< Wall > & walls, int flag);

#endif
