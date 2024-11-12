#pragma once
#include <SFML/Graphics.hpp>
#include "render.h"
#include "ai.h"
#include "utility.h"
#include "chessboard.h"
#include "state.h"

class Game
{
private:
    sf::RenderWindow window;
    Chessboard chessboard; // Move up
    AI ai;
    Render render;
    Utility utility;

    void handleEvents();
    void updateGameState();
    void initialize();

public:
    Game();
    void run();
};
