// GameGlobal.cpp
#include "GameGlobal.h"

namespace Global
{
    Maze maze(10, 20, 3, 10);
    First_Person person;
    Enemies enemy(&maze);
    std::vector<Enemies *> enemies(10);
}
