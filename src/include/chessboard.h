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

    static const std::array<std::array<std::string, 11>, 10> masculineArray;
    static const std::array<std::array<std::string, 11>, 10> feminineArray;
    static const std::array<std::array<std::string, 11>, 10> thirdArray;

    Chessboard();

    void resetBoard();
    void setBoard(const std::array<std::array<std::string, 11>, 10> &newBoard);
    void setMasculineBoard();
    void setFeminineBoard();
    void setThirdBoard();

    const std::array<std::array<std::string, 11>, 10> &getBoardState() const;
    const std::string getPiece(Types::Coord coord) const;
    void setCell(Types::Coord coord, const std::string &value);
    bool isValidCoord(Types::Coord coord) const;
    void printBoard() const;

private:
    std::array<std::array<std::string, 11>, 10> chessboard;
};

#endif // CHESSBOARD_H