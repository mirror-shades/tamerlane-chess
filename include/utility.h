// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include "types.h"
#include "gameLogic.h"
#include <SFML/Graphics.hpp>
#include <string>

class Utility
{
    GameLogic *gameLogic;
    static sf::Font font;

public:
    static Types::Coord calculateSquare(int x, int y);
    static bool clickInBoard(const int x, const int y);
    static bool checkVictoryCondition(GameLogic &gameLogic, const char &player, const char &enemy);
    int scoreMaterial();
    void undoLastMove();
    void exitToMenu();
    void handleAiVsAi();
    void initializeSounds();
    static void initializeNewGame();
    void initializeFont();
    static const sf::Font& getFont();
    void toggleSelection();
    bool clickHandler(sf::Event event, sf::RenderWindow &window);
    bool clickLogic(int x, int y);
    void handlePieceSelection(const Types::Coord &coord, const char &player);
    void handleMoves();
    void playMoveSound();
    void playCaptureSound();
    void handlePieceMovement(const std::string &_selectedPiece,
                             const Types::Coord &_selectedSquare,
                             const Types::Coord &move,
                             const char &player,
                             float score = 0.0f);

    static bool isButtonClicked(const sf::RectangleShape &button, const sf::Vector2i &mousePosition);
    static void updateGameState(const Types::Coord &initialSquare, const Types::Piece &pieceMoved, const Types::Coord &move, const std::string &target, const char &player, float score, GameLogic &gameLogic);
    static sf::RectangleShape createButton(const sf::Vector2f &size,
                                           const sf::Vector2f &position,
                                           const sf::Color &fillColor);
    static void drawButton(sf::RenderWindow &window,
                           const sf::RectangleShape &button,
                           const std::string &text,
                           unsigned int characterSize);
};