#pragma once
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "types.h"
#include <array>
#include <string>

class Chessboard
{
public:
    static const int rows = 10;
    static const int cols = 11;

    Chessboard(); // Constructor declaration
    const std::array<std::array<std::string, cols>, rows> &getBoardState() const;
    const std::string getPiece(Types::Coord coord) const;
    void setCell(Types::Coord coord, const std::string &value);
    bool isValidCoord(Types::Coord coord) const;

private:
    std::array<std::array<std::string, cols>, rows> chessboard;
};

#endif // CHESSBOARD_H