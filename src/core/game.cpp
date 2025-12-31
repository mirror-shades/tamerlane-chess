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
#include "analysis.h"
int renders = 1;

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
    static State::GameState previousState = State::GameState::Menu;
    
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type != sf::Event::MouseMoved)
        {
            State::renderNeeded = true;
        }

        // Pass events to analysis screen if in Analysis state
        if (State::state == State::GameState::Analysis)
        {
            analysis.handleEvent(event, window);
        }

        utility.clickHandler(event, window);

        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
    
    // Handle zoom transitions based on state changes
    if (previousState != State::state)
    {
        if (State::state == State::GameState::Game)
        {
            // Game started - zoom in
            render.setZoomLevel(State::ZoomLevel::ZoomedIn);
        }
        else if (State::state == State::GameState::Menu || State::state == State::GameState::AIOptions)
        {
            // Returned to menu - zoom out and show tint
            render.setZoomLevel(State::ZoomLevel::ZoomedOut);
        }
        else if (State::state == State::GameState::Analysis)
        {
            // Analysis mode - zoom out (no tint)
            render.setZoomLevel(State::ZoomLevel::ZoomedOut);
        }
        previousState = State::state;
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

    // Update camera system
    render.updateCamera(window);
    
    // Get and apply current view
    sf::View currentView = render.getCurrentView(window);
    sf::View oldView = window.getView();
    window.setView(currentView);

    window.clear(sf::Color::White);
    render.drawBackground(window, currentView);
    render.drawBoard(window);
    
    // Draw game elements that are affected by zoom
    if (State::state == State::GameState::Game)
    {
        render.highlightSquares(window);
        render.highlightPreviousMove(window);
        render.highlightKings(window);
        render.drawPieces(window, State::images);
        render.drawCapturedPieces(window, State::images);
        render.winScreen(window);
    }
    else if (State::state == State::GameState::Analysis)
    {
        // Draw board elements in analysis mode (pieces, etc.)
        render.highlightPreviousMove(window);
        render.highlightKings(window);
        render.drawPieces(window, State::images);
        render.drawCapturedPieces(window, State::images);
    }

    // Restore original view for UI elements that need window coordinates
    window.setView(oldView);
    
    // Draw UI elements that should not be affected by zoom (menus, buttons, etc.)
    if (State::state == State::GameState::Menu ||
        State::state == State::GameState::AIOptions)
    {
        // Draw tint overlay for menu
        if (State::currentZoomLevel == State::ZoomLevel::ZoomedOut)
        {
            render.tintScreen(window);
        }
        menu.drawMenuScreen(window);
    }
    else if (State::state == State::GameState::Analysis)
    {
        // Analysis UI is drawn in window coordinates (not affected by zoom)
        analysis.drawAnalysisScreen(window, render);
    }
    else if (State::state == State::GameState::Game)
    {
        // Game UI elements (exit button, etc.)
        render.drawExitButton(window);
    }

    //for centering
    //render.drawGrid(window, 75);

    window.display();

    // Keep rendering if zooming is in progress, or if we're in menu state (for shader animations)
    if (!State::isZooming && 
        State::state != State::GameState::Menu && 
        State::state != State::GameState::AIOptions)
    {
        State::renderNeeded = false;
    }
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

    // Create a default view for initialization
    sf::View defaultView = render.getCurrentView(window);
    render.drawBackground(window, defaultView);

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

    utility.initializeFont();
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

        // Render only if needed, or if zooming (to ensure smooth zoom transitions)
        if (State::renderNeeded || State::animationActive || State::isZooming)
        {
            updateGameState();
        }
    }
}
