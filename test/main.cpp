#include "Maze.h"

int main()
{
    int n, r, c;
    std::cin >> n >> r >> c;

    std::vector< Wall > punched_walls = build_maze(n, r, c);
    print_maze(n, punched_walls);
    
    return 0;
}
