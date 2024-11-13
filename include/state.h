// Copyright 2024. mirror-shades. GPL-2.0 License.
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

    static constexpr unsigned int WINDOW_WIDTH = 975;
    static constexpr unsigned int WINDOW_HEIGHT = 900;

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
    static bool renderNeeded;
    static Types::Piece selectedPiece;
    static Types::Coord selectedSquare;
    static std::vector<Types::Turn> turnHistory;
    static char player;
    static std::vector<Types::Coord> moveList;
    static std::vector<std::string> moveHistory;
    static std::vector<std::string> whitePiecesCaptured;
    static std::vector<std::string> blackPiecesCaptured;
    static std::map<std::string, sf::Sprite> images;
    static sf::Sprite backgroundSprite;
    static sf::Clock deltaClock;
    static float deltaTime;
    static bool animationActive;
    static sf::Texture backgroundTexture;
    static sf::Color colour1;
    static sf::Color colour2;
    static sf::Color colourSelected;
    static sf::Color colourPrevMove;
    static sf::Color colourMove;
};
