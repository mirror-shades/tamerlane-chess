#pragma once
#include "types.h"
#include <vector>
#include <string>

class State
{
public:
    enum class GameState
    {
        Menu,
        AIOptions,
        Game
    };

    static GameState state;
    static char winner;
    static bool gameOver;
    static bool alt;
    static bool aiActive;
    static bool aiMoveQueued;
    static bool isWhiteKingInCheck;
    static bool isBlackKingInCheck;
    static int turns;
    static int aiDifficulty;
    static bool ended;
    static bool drawPossible;
    static bool isPieceSelected;
    static std::string selectedPiece;
    static Types::Coord selectedSquare;
    static std::vector<Types::Turn> turnHistory;
    static char player;
    static std::vector<Types::Coord> moveList;
    static std::vector<std::string> moveHistory;
    static std::vector<std::string> whitePiecesCaptured;
    static std::vector<std::string> blackPiecesCaptured;
};
