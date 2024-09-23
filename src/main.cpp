// Include necessary headers
#include "include/render.h"
#include "include/ai.h"
#include "include/types.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "include/chessboard.h"
#include <iostream>

// Main function
int main()
{
    Render render;
    Utility utility;
    Chessboard chessboard;
    AI ai(chessboard);
    GameLogic gameLogic;
    // Initialize the game window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Tamerlane Chess");

    // Load and set up the background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = render.renderBackground(window, backgroundTexture);

    // Load chess piece images
    auto pieceImages = render.loadImages();

    sf::Clock frameClock;
    const sf::Time frameTime = sf::seconds(1.0f / 60.0f); // 60 FPS cap

    // FPS tracking (optional, for debugging)
    static int frameCount = 0;
    static sf::Clock fpsClock;
    static float elapsedTime = 0.0f;

    // Main game loop
    while (window.isOpen())
    {
        // Event handling
        sf::Event event;
        bool needsUpdate = false;
        while (window.pollEvent(event))
        {
            if (utility.clickHandler(event, window))
            {
                needsUpdate = true;
            }
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Update the frame if needed or if enough time has passed
        if (needsUpdate || frameClock.getElapsedTime() >= frameTime)
        {
            render.gameFrame(window, pieceImages, backgroundSprite);
            frameClock.restart();

            // FPS tracking (optional, for debugging)
            static int frameCount = 0;
            static sf::Clock fpsClock;
            static float elapsedTime = 0.0f;

            frameCount++;
            elapsedTime = fpsClock.getElapsedTime().asSeconds();
            if (elapsedTime >= 5.0f)
            {
                std::cout << "FPS: " << frameCount / elapsedTime << std::endl;
                frameCount = 0;
                fpsClock.restart();
            }
        }
        else
        {
            // Sleep to avoid excessive CPU usage
            sf::sleep(frameTime - frameClock.getElapsedTime());
        }
    }

    return 0;
}
