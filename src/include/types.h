#pragma once
#include <string>

namespace Types
{
    struct Coord
    {
        int x;
        int y;

        bool operator==(const Coord &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Coord &other) const
        {
            return !(*this == other);
        }
    };

    struct Turn
    {
        int turn;
        char player;
        Coord initialSquare;
        Coord finalSquare;
        std::string pieceMoved;
        std::string pieceCaptured;
    };
}