// Include necessary headers
#include "include/game.h"
#include "include/ai.h"
#include "include/types.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "include/chessboard.h"

// Main function
int main()
{
    Game game;
    Chessboard chessboard;
    AI ai(chessboard);
    GameLogic gameLogic;
    // Initialize the game window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Tamerlane Chess");

    // Load and set up the background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = game.renderBackground(window, backgroundTexture);

    // Load chess piece images
    auto pieceImages = game.loadImages();

    // Initialize a clock for measuring frame time
    sf::Clock deltaClock;

    // Main game loop
    while (window.isOpen())
    {
        // Calculate time since last frame
        float deltaTime = deltaClock.restart().asSeconds();

        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (!game.gameOver && !game.animationInProgress && event.mouseButton.button == sf::Mouse::Left)
                {
                    bool playerMoved = game.clickLogic(event.mouseButton.x, event.mouseButton.y);

                    if (playerMoved && game.aiActive)
                    {
                        game.aiMoveQueued = true;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.control && event.key.code == sf::Keyboard::Z)
                {
                    game.undoLastMove();
                }
            }
        }

        // Update animations
        game.updateAnimations(deltaTime);

        // Process AI move if queued and animation is finished
        if (game.aiMoveQueued && !game.animationInProgress && game.winner == '-')
        {
            char aiPlayer = (game.turns % 2 == 0) ? 'b' : 'w';
            Types::Turn aiMove = ai.minMax(gameLogic, aiPlayer, game.turns, game.alt, game.aiDifficulty,
                                           -std::numeric_limits<float>::infinity(),
                                           std::numeric_limits<float>::infinity());
            game.handlePieceMovement(aiMove.pieceMoved, aiMove.initialSquare, aiMove.finalSquare, aiPlayer);
            game.aiMoveQueued = false;
        }

        // Handle AI vs AI gameplay
        game.handleAiVsAi();

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the background
        window.draw(backgroundSprite);

        // Draw the chess board
        game.drawBoard(window);

        if (game.state == Game::GameState::Menu || game.state == Game::GameState::AIOptions)
        {
            game.drawMenuScreen(window);
        }

        else
        {
            game.highlightSquares(window);
            game.highlightPreviousMove(window);
            Types::Coord whiteKingPosition, blackKingPosition;
            gameLogic.findAndSetKingPosition(whiteKingPosition, 'w');
            gameLogic.findAndSetKingPosition(blackKingPosition, 'b');
            game.highlightKing(window, whiteKingPosition, Game::isWhiteKingInCheck);
            game.highlightKing(window, blackKingPosition, Game::isBlackKingInCheck);
            game.drawPieces(window, pieceImages);
            game.drawExitButton(window);
            game.drawCapturedPieces(window, pieceImages);
            game.winScreen(window);
        }

        // Display everything that was drawn
        window.display();
    }

    return 0;
}