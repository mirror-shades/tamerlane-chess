#pragma once

#include "types.h"
#include "gameLogic.h"
#include <SFML/Graphics.hpp>
#include <string>

class Utility
{
    GameLogic *gameLogic;

public:
    static Types::Coord calculateSquare(int x, int y);
    static bool clickInBoard(const int x, const int y);
    static bool checkVictoryCondition(GameLogic &gameLogic, const char &player, const char &enemy);
    int scoreMaterial();
    void undoLastMove();

    // New button-related functions
    static sf::RectangleShape createButton(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &fillColor);
    static void drawButton(sf::RenderWindow &window, const sf::RectangleShape &button, const std::string &text, const sf::Font &font, unsigned int characterSize);
    static bool isButtonClicked(const sf::RectangleShape &button, const sf::Vector2i &mousePosition);
    static void updateGameState(const Types::Coord &move, const std::string &target, const char &player, GameLogic &gameLogic);
};