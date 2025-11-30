#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <stack>
#include <set>
#include <vector>

class Cell
{
public:
    Cell(int r = 0, int c = 0)
        : r_(r), c_(c){// , open_neigbhors_(new Cell *[4])
    // {
    //     for (int i = 0; i < 4; ++i)
    //     {
    //         open_neigbhors_[i] = NULL;
    //     }
        std::cout << r << ' ' << c << std::endl;
    }
    Cell(const Cell & c)
        : r_(c.r_), c_(c.c_)
    {
        std::cout << "copy constructor" << std::endl;
    }
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

    bool operator<(const Cell& other) const {
        if (r_ != other.r_) return r_ < other.r_;
        return c_ < other.c_;
    }
    int r_, c_;

    // std::vector< Cell * > open_neigbhors_;
    //Cell ** open_neigbhors_;
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
    Wall(const Cell & cel, const Cell & cell)
        : c0(cel), c1(cell)
    {
        std::cout << "here1?\n";
    }
    Wall(const int r0, const int c0, const int r1, const int c1)
        : c0(r0, c0), c1(r1, c1)
    {
        std::cout << "here2?\n";
    }
    Wall(const Wall & w)
        : c0(w.c0), c1(w.c1)
    {
        std::cout << c0 << ' ' << c1 << std::endl;;
    }
    Wall & operator=(const Wall & w)
    {
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
    Cell c0, c1;
};

inline
std::ostream & operator<<(std::ostream & cout, const Wall & w)
{
    std::cout << "[" << w.c0 << ',' << w.c1 << "]";
    return cout;
}

std::vector< Wall > build_maze(int n, int r, int c);
void print_maze(int n, const std::vector<Wall> & p);

#endif
