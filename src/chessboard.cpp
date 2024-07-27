#include "globals.h"
#include "chessboard.h"

// Define the global chessboard variable
Chessboard chessboard;

Chessboard::Chessboard()
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

const std::string Chessboard::getPiece(Types::Coord coord) const
{
    return chessboard[coord.y][coord.x];
}

void Chessboard::setCell(Types::Coord coord, const std::string &value)
{
    if (coord.x >= 0 && coord.x < rows && coord.y >= 0 && coord.y < cols)
    {
        chessboard[coord.y][coord.x] = value;
    }
}