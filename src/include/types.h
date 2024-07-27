#pragma once

namespace Types
{
    struct Coord
    {
        int x;
        int y;

        // Overload == operator to allow comparison of Coord objects
        bool operator==(const Coord &other) const
        {
            return x == other.x && y == other.y;
        }
    };
}