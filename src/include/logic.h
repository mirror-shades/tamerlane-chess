#pragma once
#include <vector>
#include <string>
#include "types.h"
#include "chessboard.h"

class Logic
{
public:
    std::vector<Types::Coord> getPawnMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getRookMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getTaliaMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getElephantMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getVizierMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getKhanMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getWarEngineMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getAdminMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getMongolMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getCamelMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getGiraffeMoves(Types::Coord coord, char player);

    std::vector<std::pair<std::string, std::vector<Types::Coord>>> getAllMoves(char player);
    std::vector<Types::Coord> getMoves(Types::Coord coord, std::string piece, char player);
    std::vector<Types::Coord> filterLegalMoves(const std::vector<Types::Coord> &possibleMoves, const Types::Coord &fromCoord, const std::string &piece, char player);
    void findAndSetKingPosition(Types::Coord &kingPosition, const char &player);
    void promotePawns(char player);
    bool isKingInCheck(const char &player, auto boardState);
    bool hasLegalMoves(char player);
};