#include "globals.h"
#include "chessboard.h"

// Define the global chessboard variable
Chessboard chessboard;

Chessboard::Chessboard()
{
    // chessboard = {{{"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
    //                 {"bRk", "bMo", "bTa", "bGi", "bVi", "bKa", "bAd", "bGi", "bTa", "bMo", "bRk"},
    //                 {"bpR", "bpM", "bpT", "bpG", "bpV", "bpK", "bpA", "bpE", "bpC", "bpW", "bp0"},
    //                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                 {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    //                 {"wp0", "wpW", "wpC", "wpE", "wpA", "wpK", "wpV", "wpG", "wpT", "wpM", "wpR"},
    //                 {"wRk", "wMo", "wTa", "wGi", "wAd", "wKa", "wVi", "wGi", "wTa", "wMo", "wRk"},
    //                 {"wEl", "---", "wCa", "---", "wWe", "---", "wWe", "---", "wCa", "---", "wEl"}}};

    chessboard = {{{"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
                   {"bRk", "---", "bTa", "bGi", "bVi", "bKa", "bAd", "bGi", "bTa", "bMo", "bRk"},
                   {"bpR", "---", "bpT", "bpG", "bpV", "bpK", "bpA", "bpE", "bpC", "bpW", "bp0"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"---", "wRk", "---", "---", "wEl", "---", "---", "---", "---", "---", "---"},
                   {"---", "---", "---", "---", "---", "wGi", "---", "---", "---", "---", "---"},
                   {"---", "---", "wCa", "---", "---", "---", "---", "---", "---", "---", "---"},
                   {"wp0", "---", "---", "wpE", "wpA", "---", "wpV", "wpG", "wpT", "wpM", "wpR"},
                   {"wRk", "bpT", "wTa", "wGi", "wAd", "wKa", "wVi", "wGi", "wTa", "wMo", "wRk"},
                   {"---", "---", "---", "---", "---", "---", "---", "---", "wCa", "---", "wEl"}}};
}

const std::array<std::array<std::string, Chessboard::cols>, Chessboard::rows> &Chessboard::getBoardState() const
{
    return chessboard;
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