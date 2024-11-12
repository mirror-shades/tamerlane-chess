// Include necessary headers
#include "render.h"
#include "ai.h"
#include "types.h"
#include "utility.h"
#include "chessboard.h"
#include "state.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int renders = 1;

class Game
{
private:
    sf::RenderWindow window;
    Render render;
    Utility utility;
    Chessboard chessboard;
    AI ai;
    sf::Sprite backgroundSprite;

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type != sf::Event::MouseMoved)
            {
                State::renderNeeded = true;
            }

            utility.clickHandler(event, window);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    void updateGameState()
    {
        // Limit to 60 FPS
        static sf::Clock frameClock;
        const float frameTime = 1.0f / 120.0f;
        if (frameClock.getElapsedTime().asSeconds() < frameTime)
        {
            return;
        }
        frameClock.restart();

        window.clear(sf::Color::White);
        render.drawBackground(window);
        render.drawBoard(window);
        render.drawMenuScreen(window);
        render.renderGameElements(window);
        window.display();

        // BEGIN RENDER COUNT
        renders++;
        std::cout << "Total renders: " << renders << std::endl;
        // END RENDER COUNT

        State::renderNeeded = false;
    }

    void initialize()
    {
        render.drawBackground(window);
        State::images = render.loadImages(window);
        utility.initializeSounds();
        State::renderNeeded = true;
    }

public:
    Game() : window(sf::VideoMode(975, 900), "Tamerlane Chess"),
             ai(chessboard) {}

    void run()
    {
        initialize();

        while (window.isOpen())
        {
            handleEvents();

            // Always update game logic, regardless of rendering
            utility.handleMoves();

            // Render only if needed
            if (State::renderNeeded || State::animationActive)
            {
                updateGameState();
            }
        }
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
