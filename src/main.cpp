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

void _gameRender(sf::RenderWindow &window, auto render, auto utility, auto backgroundSprite, auto pieceImages)
{
    utility.handleMoves();

    // Clear the window
    window.clear(sf::Color::White);

    // Draw the background
    window.draw(backgroundSprite);

    // Draw the chess board
    render.drawBoard(window);

    render.drawMenuScreen(window);

    render.gameHandler(window, pieceImages);

    // Display everything that was drawn
    window.display();
}

void _gameFrame(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages, auto utility, auto render)
{
    utility.handleMoves();

    render.gameHandler(window, pieceImages);
}

// Main function
int main()
{
    Render render;
    Utility utility;
    Chessboard chessboard;
    AI ai(chessboard);
    // Initialize the game window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Tamerlane Chess");

    // Load and set up the background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = render.renderBackground(window, backgroundTexture);

    // Load chess piece images
    auto pieceImages = render.loadImages();
    // Initialize sounds
    utility.initializeSounds();

    sf::Clock frameClock;

    // FPS tracking (optional, for debugging)
    static sf::Clock fpsClock;

    bool needsUpdate = true; // Initial render

    // Main game loop
    while (window.isOpen())
    {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Always set needsUpdate when handling clicks, regardless of the return value
            utility.clickHandler(event, window);
            needsUpdate = true; // Set update flag for any user interaction

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Update the frame if needed (user input or animation)
        if (needsUpdate || State::animationActive)
        {
            _gameRender(window, render, utility, backgroundSprite, pieceImages);
            _gameFrame(window, pieceImages, utility, render);

            // Only reset the update flag if there's no animation running
            if (!State::animationActive)
            {
                needsUpdate = false;
            }
        }
    }

    return 0;
}
