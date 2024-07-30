#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include <iostream>

const int squareSize = 75;

Types::Coord Utility::calculateSquare(int x, int y)
{
    int _x = x / squareSize - 1;
    int _y = y / squareSize;
    return {_x, _y};
}

bool Utility::clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    return !(x < boardOffset || x > boardWidth || y > boardHeight);
}
