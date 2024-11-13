// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <iostream>
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
               ai(chessboard),
               render(),
               menu(),
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
    menu.drawMenuScreen(window);
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
        std::cerr << "Failed to load icon: assets/images/icon.png" << std::endl;
    }

    render.drawBackground(window);

    // Add error handling for image loading
    try
    {
        State::images = render.loadImages(window);
        if (State::images.empty())
        {
            throw std::runtime_error("No images were loaded successfully");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error loading game assets: "
                  << e.what()
                  << std::endl;
        window.close();
        return;
    }

    utility.initializeSounds();
    State::renderNeeded = true;
}

void Game::run()
{
    initialize();

    // Only enter game loop if window is open and initialization succeeded
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
