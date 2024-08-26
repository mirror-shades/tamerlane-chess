#include "globals.h"
#include "chessboard.h"

// Define the global chessboard variable
Chessboard chessboard;

#include <array>
#include <string>
#include <iostream>

const std::array<std::array<std::string, 11>, 10> masculineArray = {{{"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
                                                                     {"bRk", "bMo", "bTa", "bGi", "bVi", "bKa", "bAd", "bGi", "bTa", "bMo", "bRk"},
                                                                     {"bpR", "bpM", "bpT", "bpG", "bpV", "bpK", "bpA", "bpE", "bpC", "bpW", "bp0"},
                                                                     {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                     {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                     {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                     {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                     {"wp0", "wpW", "wpC", "wpE", "wpA", "wpK", "wpV", "wpG", "wpT", "wpM", "wpR"},
                                                                     {"wRk", "wMo", "wTa", "wGi", "wAd", "wKa", "wVi", "wGi", "wTa", "wMo", "wRk"},
                                                                     {"wEl", "---", "wCa", "---", "wWe", "---", "wWe", "---", "wCa", "---", "wEl"}}};

const std::array<std::array<std::string, 11>, 10> feminineArray = {{{"bEl", "---", "bCa", "---", "bVi", "bKa", "bAd", "---", "bCa", "---", "bEl"},
                                                                    {"bRk", "bMo", "bTa", "bGi", "bWe", "bpK", "bWe", "bGi", "bTa", "bMo", "bRk"},
                                                                    {"bpR", "bpM", "bpT", "bpG", "bpV", "---", "bpA", "bpE", "bpC", "bpW", "bp0"},
                                                                    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                    {"wp0", "wpW", "wpC", "wpE", "wpA", "---", "wpV", "wpG", "wpT", "wpM", "wpR"},
                                                                    {"wRk", "wMo", "wTa", "wGi", "wWe", "wpK", "wWe", "wGi", "wTa", "wMo", "wRk"},
                                                                    {"wEl", "---", "wCa", "---", "wAd", "wKa", "wVi", "---", "wCa", "---", "wEl"}}};

const std::array<std::array<std::string, 11>, 10> thirdArray = {{{"bEl", "---", "bCa", "---", "bVi", "bKa", "bAd", "---", "bCa", "---", "bEl"},
                                                                 {"bRk", "bMo", "bWe", "bTa", "bGi", "bpK", "bGi", "bTa", "bWe", "bMo", "bRk"},
                                                                 {"bpR", "bpM", "bpT", "bpG", "bpV", "---", "bpA", "bpE", "bpC", "bpW", "bp0"},
                                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                 {"wp0", "wpW", "wpC", "wpE", "wpA", "---", "wpV", "wpG", "wpT", "wpM", "wpR"},
                                                                 {"wRk", "wMo", "wWe", "wTa", "wGi", "wpK", "wGi", "wTa", "wWe", "wMo", "wRk"},
                                                                 {"wEl", "---", "wCa", "---", "wAd", "wKa", "wVi", "---", "wCa", "---", "wEl"}}};

const std::array<std::array<std::string, 11>, 10> testBoard = {{{"---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---", "---"},
                                                                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                {"---", "wVi", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                {"wKa", "---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---"},
                                                                {"---", "---", "bTa", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                {"---", "---", "---", "---", "bGi", "---", "---", "---", "---", "---", "---"},
                                                                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "bKa"},
                                                                {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                                                                {"bKa", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"}}};

Chessboard::Chessboard()
{
    chessboard = {{{"---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"wKa", "---", "---", "---", "---", "---", "---", "---", "---", "wRk", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "bKa"},
                   {"---", "bRk", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "bRk", "---", "---", "---", "---", "---", "---", "---", "---"}}};
}

void Chessboard::resetBoard()
{
    chessboard = {{{"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
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

const std::array<std::array<std::string, Chessboard::cols>, Chessboard::rows> &Chessboard::getBoardState() const
{
    return chessboard;
}

void Chessboard::setBoard(const std::array<std::array<std::string, 11>, 10> &newBoard)
{
    chessboard = newBoard;
}

const std::string Chessboard::getPiece(Types::Coord coord) const
{
    if (isValidCoord(coord))
    {
        return chessboard[coord.y][coord.x];
    }
    return "Invalid"; // or handle the invalid case appropriately
}

void Chessboard::setCell(Types::Coord coord, const std::string &value)
{
    if (isValidCoord(coord))
    {
        chessboard[coord.y][coord.x] = value;
    }
}

bool Chessboard::isValidCoord(Types::Coord coord) const
{
    return coord.x >= 0 && coord.x < Chessboard::cols && coord.y >= 0 && coord.y < Chessboard::rows;
}

void Chessboard::printBoard() const
{
    for (const auto &row : chessboard)
    {
        for (const auto &cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}