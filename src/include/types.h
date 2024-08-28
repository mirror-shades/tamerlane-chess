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

        bool operator==(const std::initializer_list<int> &il) const
        {
            return il.size() == 2 && x == *il.begin() && y == *(il.begin() + 1);
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
        float score;
    };
}