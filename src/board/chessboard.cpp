// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <array>
#include <iostream>
#include "globals.h"
#include "chessboard.h"

Chessboard chessboard;

void Chessboard::setBoard(const Types::Board &newBoard)
{
    chessboard = newBoard;
}

void Chessboard::resetBoard()
{
    setMasculineBoard();
}

void Chessboard::setMasculineBoard()
{
    chessboard.board =
        {{{"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
          {"bRk", "bMo", "bTa", "bGi", "bVi", "bKa", "bAd", "bGi", "bTa", "bMo", "bRk"},
          {"bpR", "bpM", "bpT", "bpG", "bpV", "bpK", "bpA", "bpE", "bpC", "bpW", "bp0"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"wp0", "wpW", "wpC", "wpE", "wpA", "wpK", "wpV", "wpG", "wpT", "wpM", "wpR"},
          {"wRk", "wMo", "wTa", "wGi", "wAd", "wKa", "wVi", "wGi", "wTa", "wMo", "wRk"},
          {"wEl", "---", "wCa", "---", "wWe", "---", "wWe", "---", "wCa", "---", "wEl"}}};
}

void Chessboard::setFeminineBoard()
{
    chessboard.board =
        {{{"bEl", "---", "bCa", "---", "bVi", "bKa", "bAd", "---", "bCa", "---", "bEl"},
          {"bRk", "bMo", "bTa", "bGi", "bWe", "bpK", "bWe", "bGi", "bTa", "bMo", "bRk"},
          {"bpR", "bpM", "bpT", "bpG", "bpV", "---", "bpA", "bpE", "bpC", "bpW", "bp0"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"wp0", "wpW", "wpC", "wpE", "wpA", "---", "wpV", "wpG", "wpT", "wpM", "wpR"},
          {"wRk", "wMo", "wTa", "wGi", "wWe", "wpK", "wWe", "wGi", "wTa", "wMo", "wRk"},
          {"wEl", "---", "wCa", "---", "wAd", "wKa", "wVi", "---", "wCa", "---", "wEl"}}};
}

void Chessboard::setThirdBoard()
{
    chessboard.board =
        {{{"bEl", "---", "bCa", "---", "bVi", "bKa", "bAd", "---", "bCa", "---", "bEl"},
          {"bRk", "bMo", "bWe", "bTa", "bGi", "bpK", "bGi", "bTa", "bWe", "bMo", "bRk"},
          {"bpR", "bpM", "bpT", "bpG", "bpV", "---", "bpA", "bpE", "bpC", "bpW", "bp0"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
          {"wp0", "wpW", "wpC", "wpE", "wpA", "---", "wpV", "wpG", "wpT", "wpM", "wpR"},
          {"wRk", "wMo", "wWe", "wTa", "wGi", "wpK", "wGi", "wTa", "wWe", "wMo", "wRk"},
          {"wEl", "---", "wCa", "---", "wAd", "wKa", "wVi", "---", "wCa", "---", "wEl"}}};
}

const Types::Board &Chessboard::getBoardState() const
{
    return chessboard;
}

const Types::Piece Chessboard::getPiece(Types::Coord coord) const
{
    if (isValidCoord(coord))
    {
        return chessboard.board[coord.y][coord.x];
    }
    return "Invalid";
}

void Chessboard::setCell(Types::Coord coord, const Types::Piece &value)
{
    if (isValidCoord(coord))
    {
        chessboard.board[coord.y][coord.x] = value;
    }
}

bool Chessboard::isValidCoord(Types::Coord coord) const
{
    return coord.x >= 0 && coord.x < Chessboard::cols &&
           coord.y >= 0 && coord.y < Chessboard::rows;
}

void Chessboard::printBoard() const
{
    for (const auto &row : chessboard.board)
    {
        for (const auto &cell : row)
        {
            std::cout << cell.code << " ";
        }
        std::cout << std::endl;
    }
}

Chessboard::Chessboard()
{
    setMasculineBoard();

    // for testing
    // chessboard = {{{"---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---", "---"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                {"---", "wVi", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                {"wKa", "---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                {"---", "---", "---", "---", "bGi", "---", "---", "---", "---", "---", "---"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "bKa"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"}}};
}
