#pragma once
#include "types.h"
#include <iostream>
#include <vector>

class Logic
{
public:
    std::vector<Types::Coord> getPawnMoves(Types::Coord coord, char player);
};