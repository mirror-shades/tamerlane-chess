#pragma once
#include <vector>
#include <string>
#include "types.h"
#include "chessboard.h"

class GameLogic
{
public:
    std::vector<std::pair<std::string, std::vector<Types::Coord>>> getAllMoves(char player, bool alt);
    std::vector<Types::Coord> getMoves(Types::Coord coord, std::string piece, char player, bool alt);
    std::vector<Types::Coord> filterLegalMoves(const std::vector<Types::Coord> &possibleMoves, const Types::Coord &fromCoord, const std::string &piece, char player, bool alt);
    Types::Coord findPawnX(char player);
    void findAndSetKingPosition(Types::Coord &kingPosition, const char &player);
    void promotePawns(char player);
    void checkPawnForks(char player);
    bool isKingInCheck(const char &player, auto boardState, bool alt);
    bool hasLegalMoves(char player, bool alt);
};