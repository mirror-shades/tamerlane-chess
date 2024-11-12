#pragma once
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "types.h"
#include <array>

class Chessboard
{
public:
    // Constants for the Tamerlane Chess board
    static const int squareSize = 75;
    static const int rows = 10;
    static const int cols = 11;

    static const std::array<std::array<Types::Piece, 11>, 10> masculineArray;
    static const std::array<std::array<Types::Piece, 11>, 10> feminineArray;
    static const std::array<std::array<Types::Piece, 11>, 10> thirdArray;

    Chessboard();
    void resetBoard();
    void setBoard(const std::array<std::array<Types::Piece, 11>, 10> &newBoard);
    void setMasculineBoard();
    void setFeminineBoard();
    void setThirdBoard();

    const std::array<std::array<Types::Piece, 11>, 10> &getBoardState() const;
    const Types::Piece getPiece(Types::Coord coord) const;
    void setCell(Types::Coord coord, const Types::Piece &value);
    bool isValidCoord(Types::Coord coord) const;
    void printBoard() const;

private:
    std::array<std::array<Types::Piece, 11>, 10> chessboard;
};

#endif // CHESSBOARD_H