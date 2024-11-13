// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include <array>
#include "types.h"

class Chessboard
{
public:
    // Constants for the Tamerlane Chess board
    static const int squareSize = 75;
    static const int rows = 10;
    static const int cols = 11;

    static const Types::Board masculineArray;
    static const Types::Board feminineArray;
    static const Types::Board thirdArray;

    Chessboard();
    void resetBoard();
    void setBoard(const Types::Board &newBoard);
    void setMasculineBoard();
    void setFeminineBoard();
    void setThirdBoard();

    const Types::Board &getBoardState() const;
    const Types::Piece getPiece(Types::Coord coord) const;
    void setCell(Types::Coord coord, const Types::Piece &value);
    bool isValidCoord(Types::Coord coord) const;
    void printBoard() const;

private:
    Types::Board chessboard;
};
