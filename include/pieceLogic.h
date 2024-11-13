// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include <vector>
#include <string>
#include "types.h"
#include "chessboard.h"

class PieceLogic
{
public:
    const std::vector<Types::Coord> ORTHOGONAL = {
        // up
        Types::Coord(0, -1),
        // down
        Types::Coord(0, 1),
        // left
        Types::Coord(-1, 0),
        // right
        Types::Coord(1, 0)};

    const std::vector<Types::Coord> DIAGONAL = {
        // down left
        Types::Coord(-1, 1),
        // down right
        Types::Coord(1, 1),
        // up left
        Types::Coord(-1, -1),
        // up right
        Types::Coord(1, -1)};

    // constructs a vector of all directions to keep this logic out of the cpp
    const std::vector<Types::Coord> ALL_DIRECTIONS = [this]()
    {
        std::vector<Types::Coord> dirs(ORTHOGONAL);
        dirs.insert(dirs.end(), DIAGONAL.begin(), DIAGONAL.end());
        return dirs;
    }();

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

    std::vector<Types::Coord> getAltWarEngineMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getAltElephantMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getAltAdminMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getAltVizierMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getAltPawnMoves(Types::Coord coord, char player);
};