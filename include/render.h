// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include "gameLogic.h"
#include "utility.h"
#include <SFML/Graphics.hpp>
#include "types.h"

class Render
{
    GameLogic *gameLogic;
    Utility *utility;

public:
    static bool animationInProgress;
    static Types::Coord move;

    void drawBackground(sf::RenderWindow &window);
    std::map<std::string, sf::Sprite> loadImages(sf::RenderWindow &window);
    void updateAnimations();
    void drawBoard(sf::RenderWindow &window);
    void drawExitButton(sf::RenderWindow &window);
    void winScreen(sf::RenderWindow &window);
    void highlightSquares(sf::RenderWindow &window);
    void highlightPreviousMove(sf::RenderWindow &window);
    void highlightKing(sf::RenderWindow &window, Types::Coord kingPosition, bool isInCheck);
    void drawPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void drawCapturedPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration);
    void renderGameElements(sf::RenderWindow &window);
    void highlightKings(sf::RenderWindow &window);
    void tintScreen(sf::RenderWindow &window);
    std::string findAssetsPath(const std::string &filename);

private:
    sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t);
    void highlightSquare(sf::RenderWindow &window, const Types::Coord &coord);
};
