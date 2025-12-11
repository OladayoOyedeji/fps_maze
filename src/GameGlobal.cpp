// GameGlobal.cpp
#include "GameGlobal.h"

namespace Global
{
    Maze maze(10, 10, 3, 10);
    First_Person person;
    Enemies enemy(&maze);
    std::vector<Enemies *> enemies(10);
}
