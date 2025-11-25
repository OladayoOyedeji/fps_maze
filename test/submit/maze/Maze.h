#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include "rlist.h"
#include <vector>
//#include <std>

class Cell
{
public:
    Cell(int r = 0, int c = 0)
        : r_(r), c_(c)
    {}
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
    int r_, c_;
};
class PunchedWall
{
public:
    PunchedWall(const Cell & cel, const Cell & cell)
        : c0(cel), c1(cell)
    {}
    PunchedWall(const int r0, const int c0, const int r1, const int c1)
        : c0(r0, c0), c1(r1, c1)
    {}
    bool operator==(const PunchedWall & p) const
    {
        return ((p.c0 == c0 && p.c1 == c1) || (p.c0 == c1 && p.c1 == c0));
    }
    Cell c0, c1;
};

std::vector< PunchedWall > build_maze(int n, int r, int c);
void print_maze(int n, const std::vector<PunchedWall> & v);

#endif
