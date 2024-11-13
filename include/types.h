// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include <string>
#include <cstring>
#include <array>

namespace Types
{
    // used to store the piece code in the selected piece
    // this is a char array to avoid global string issues
    struct Piece
    {
        char code[4]; // 3 chars + null terminator

        // Default constructor - initializes to "---"
        Piece()
        {
            strcpy(code, "---");
        }

        // Constructor from C-string
        Piece(const char *str)
        {
            strncpy(code, str, 3);
            code[3] = '\0';
        }

        // Constructor from std::string
        Piece(const std::string &str)
        {
            strncpy(code, str.c_str(), 3);
            code[3] = '\0';
        }

        // Comparison operators
        bool operator==(const Piece &other) const
        {
            return strncmp(code, other.code, 3) == 0;
        }

        bool operator==(const char *str) const
        {
            return strncmp(code, str, 3) == 0;
        }

        bool operator!=(const Piece &other) const
        {
            return !(*this == other);
        }

        // Conversion to string
        std::string toString() const
        {
            return std::string(code);
        }

        // Get individual characters
        char color() const { return code[0]; }
        char piece() const { return code[1]; }
        char variant() const { return code[2]; }
    };

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
        Piece pieceMoved;
        Piece pieceCaptured;
        float score;
    };

    struct Board
    {
        std::array<std::array<Piece, 11>, 10> board;
    };
}
