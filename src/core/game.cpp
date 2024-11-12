#include "render.h"
#include "ai.h"
#include "types.h"
#include "utility.h"
#include "chessboard.h"
#include "state.h"
#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int renders = 1;

// Add at the top with other methods
Game::Game() : window(sf::VideoMode(State::WINDOW_WIDTH, State::WINDOW_HEIGHT), "Tamerlane Chess"),
               chessboard(),
               ai(chessboard), // Initialize AI with chessboard reference
               render(),
               utility()
{
}

void Game::handleEvents()
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

void Game::updateGameState()
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

    State::renderNeeded = false;
}

void Game::initialize()
{
    // Load window icon
    sf::Image icon;
    if (icon.loadFromFile("assets/images/icon.png"))
    {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    else
    {
        // Handle error
        // std::cerr << "Failed to load icon.png" << std::endl;
    }

    render.drawBackground(window);
    State::images = render.loadImages(window);
    utility.initializeSounds();
    State::renderNeeded = true;
}

void Game::run()
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