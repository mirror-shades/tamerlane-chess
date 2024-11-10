#pragma once
#include "types.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

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
    static sf::Clock aiVsAiClock;
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
    static bool aiVsAiMode;
    static std::string selectedPiece;
    static Types::Coord selectedSquare;
    static std::vector<Types::Turn> turnHistory;
    static char player;
    static std::vector<Types::Coord> moveList;
    static std::vector<std::string> moveHistory;
    static std::vector<std::string> whitePiecesCaptured;
    static std::vector<std::string> blackPiecesCaptured;
    static sf::Clock deltaClock;
    static float deltaTime;
};