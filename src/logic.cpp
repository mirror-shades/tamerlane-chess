#include "include/logic.h"
#include "include/globals.h"

std::vector<Types::Coord> Logic::getPawnMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord forwardMove = {coord.x, coord.y + direction};
    if (forwardMove.y >= 0 && forwardMove.y < Chessboard::cols && chessboard.getPiece(forwardMove) == "---")
    {
        moves.push_back(forwardMove);
    }

    Types::Coord leftCapture = {coord.x - 1, coord.y + direction};
    if (leftCapture.x >= 0 && leftCapture.x < Chessboard::rows &&
        leftCapture.y >= 0 && leftCapture.y < Chessboard::cols &&
        chessboard.getPiece(leftCapture)[0] == enemy)
    {
        moves.push_back(leftCapture);
    }

    Types::Coord rightCapture = {coord.x + 1, coord.y + direction};
    if (rightCapture.x >= 0 && rightCapture.x <= Chessboard::rows &&
        rightCapture.y >= 0 && rightCapture.y < Chessboard::cols &&
        chessboard.getPiece(rightCapture)[0] == enemy)
    {
        moves.push_back(rightCapture);
    }

    return moves;
}