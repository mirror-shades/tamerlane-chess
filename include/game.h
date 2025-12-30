// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include <SFML/Graphics.hpp>
#include "render.h"
#include "ai.h"
#include "utility.h"
#include "chessboard.h"
#include "state.h"
#include "menu.h"
#include "analysis.h"

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Chessboard chessboard;
    AI ai;
    Render render;
    Menu menu;
    Utility utility;
    Analysis analysis;

    void handleEvents();
    void updateGameState();
    void initialize();
};
