// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include "gameLogic.h"
#include "utility.h"
#include "state.h"
#include <SFML/Graphics.hpp>
#include "types.h"

class Render
{
    GameLogic *gameLogic;
    Utility *utility;

public:
    static bool animationInProgress;
    static Types::Coord move;

    void drawBackground(sf::RenderWindow &window, const sf::View &view);
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
    void highlightKings(sf::RenderWindow &window);
    void tintScreen(sf::RenderWindow &window);
    std::string findAssetsPath(const std::string &filename);
    void drawAnalysisMenu(sf::RenderWindow &window);
    void drawGrid(sf::RenderWindow &window, int px);
    
    // Camera/zoom system
    void updateCamera(sf::RenderWindow &window);
    void setZoomLevel(State::ZoomLevel targetZoom);
    sf::View getCurrentView(sf::RenderWindow &window);
    static constexpr float ZOOMED_OUT_SCALE = 0.75f; // Scale factor for zoomed out view
    static constexpr float ZOOM_TRANSITION_TIME = 0.5f; // Seconds for zoom transition

private:
    sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t);
    void highlightSquare(sf::RenderWindow &window, const Types::Coord &coord);
    sf::Clock zoomClock;
};
