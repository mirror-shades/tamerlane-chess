#pragma once
#include "types.h"
#include <iostream>
#include <vector>

class Logic
{
public:
    std::vector<Types::Coord> getPawnMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getRookMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getTaliaMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getKhanMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getElephantMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getVizierMoves(Types::Coord coord, char player);
    std::vector<Types::Coord> getWarEngineMoves(Types::Coord coord, char player);
};