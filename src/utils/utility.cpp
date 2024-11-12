// Copyright 2024. mirror-shades. GPL-2.0 License.

#include <iostream>
#include <map>
#include <limits>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include "types.h"
#include "globals.h"
#include "utility.h"
#include "render.h"
#include "gameLogic.h"
#include "state.h"
#include "ai.h"

// Add these as member variables in the Utility class or as global variables
sf::SoundBuffer moveSoundBuffer;
sf::SoundBuffer captureSoundBuffer;
sf::Sound moveSound;
sf::Sound captureSound;

AI ai(chessboard);
Render *render;
const int squareSize = 75;

Types::Coord Utility::calculateSquare(int x, int y)
{
    return {x / squareSize - 1, y / squareSize};
}

bool Utility::clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    return !(x < boardOffset || x > boardWidth || y > boardHeight);
}

// Toggle piece selection
void Utility::toggleSelection()
{
    State::isPieceSelected = false;
    State::moveList.clear();
    State::selectedSquare = {-1, -1};
}

sf::RectangleShape Utility::createButton(
    const sf::Vector2f &size,
    const sf::Vector2f &position,
    const sf::Color &fillColor)
{
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(fillColor);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    return button;
}

void Utility::drawButton(
    sf::RenderWindow &window,
    const sf::RectangleShape &button,
    const std::string &text,
    const sf::Font &font,
    unsigned int characterSize)
{
    window.draw(button);

    sf::Text buttonText(text, font, characterSize);
    buttonText.setFillColor(sf::Color::Black);

    // Center the text on the button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(
        textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(
        button.getPosition().x + button.getSize().x / 2.0f,
        button.getPosition().y + button.getSize().y / 2.0f);

    window.draw(buttonText);
}

bool Utility::isButtonClicked(
    const sf::RectangleShape &button,
    const sf::Vector2i &mousePosition)
{
    return button.getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

// Check if the game has ended (checkmate or stalemate)
bool Utility::checkVictoryCondition(
    GameLogic &gameLogic,
    const char &player,
    const char &enemy)
{
    auto boardState = chessboard.getBoardState();
    bool hasLegalMoves = gameLogic.hasLegalMoves(enemy, State::alt);
    bool kingInCheck = gameLogic.isKingInCheck(enemy, boardState, State::alt);

    if (!hasLegalMoves)
    {
        if (kingInCheck)
        {
            State::winner = player;
            std::cout << player << " has won by checkmate" << std::endl;
        }
        else
        {
            State::winner = 's';
            std::cout << "The game is a draw by stalemate" << std::endl;
        }
        State::gameOver = true;
        return true;
    }

    return false;
}

// Update game state after a move
void Utility::updateGameState(
    const Types::Coord &move,
    const std::string &target,
    GameLogic &gameLogic)
{
    auto boardState = chessboard.getBoardState();

    State::isWhiteKingInCheck = gameLogic.isKingInCheck(
        'w',
        boardState,
        State::alt);
    State::isBlackKingInCheck = gameLogic.isKingInCheck(
        'b',
        boardState,
        State::alt);

    Types::Turn newTurn = {
        State::turns,
        State::player,
        State::selectedSquare,
        move,
        State::selectedPiece,
        target,
        0.0f};

    State::turnHistory.push_back(newTurn);
    State::turns++;

    State::isPieceSelected = false;
    State::moveList.clear();
    State::selectedSquare = {-1, -1};
}

// Undo the last move
void Utility::undoLastMove()
{
    if (!State::turnHistory.empty())
    {
        Types::Turn lastTurn = State::turnHistory.back();
        State::turnHistory.pop_back();
        chessboard.setCell(lastTurn.finalSquare, lastTurn.pieceCaptured);
        chessboard.setCell(lastTurn.initialSquare, lastTurn.pieceMoved);
        State::turns--;
        auto boardState = chessboard.getBoardState();

        State::isWhiteKingInCheck = gameLogic->isKingInCheck(
            'w',
            boardState,
            State::alt);
        State::isBlackKingInCheck = gameLogic->isKingInCheck(
            'b',
            boardState,
            State::alt);

        State::isPieceSelected = false;
        State::moveList.clear();
        State::selectedSquare = {-1, -1};

        std::cout << "Undo move: "
                  << lastTurn.pieceMoved.toString()
                  << " from ("
                  << lastTurn.finalSquare.x
                  << ", "
                  << lastTurn.finalSquare.y
                  << ") to ("
                  << lastTurn.initialSquare.x
                  << ", "
                  << lastTurn.initialSquare.y
                  << ")" << std::endl;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    }
}

int Utility::scoreMaterial()
{
    std::map<char, int> pieceValues = {
        {'p', 1},
        {'E', 1.5},
        {'W', 1.5},
        {'A', 2},
        {'V', 2},
        {'C', 3},
        {'M', 3},
        {'T', 3},
        {'G', 4},
        {'R', 5},
        {'K', 100}};
    int whiteScore = 0, blackScore = 0;

    for (const auto &piece : State::whitePiecesCaptured)
    {
        blackScore += pieceValues[piece[1]];
    }
    for (const auto &piece : State::blackPiecesCaptured)
    {
        whiteScore += pieceValues[piece[1]];
    }

    return whiteScore - blackScore;
}

// Exit to menu
void Utility::exitToMenu()
{
    std::cout << "Exiting game" << std::endl;
    // Reset game state and return to menu
    State::state = State::GameState::Menu;
    State::gameOver = false;
    State::isPieceSelected = false;
    State::moveList.clear();
    State::selectedSquare = {-1, -1};
    chessboard.resetBoard();
    State::turnHistory.clear();
    State::turns = 1;
    State::drawPossible = false;
    State::isWhiteKingInCheck = false;
    State::isBlackKingInCheck = false;
    State::winner = '-';
    State::aiVsAiMode = false;
    State::whitePiecesCaptured = {};
    State::blackPiecesCaptured = {};
}

// In the Utility class constructor or initialization method
void Utility::initializeSounds()
{
    if (!moveSoundBuffer.loadFromFile("assets/audio/move.mp3"))
    {
        std::cerr << "Failed to load move sound" << std::endl;
    }
    if (!captureSoundBuffer.loadFromFile("assets/audio/capture.mp3"))
    {
        std::cerr << "Failed to load capture sound" << std::endl;
    }
    moveSound.setBuffer(moveSoundBuffer);
    captureSound.setBuffer(captureSoundBuffer);
}

bool Utility::clickHandler(sf::Event event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::Closed)
        window.close();

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (
            !State::gameOver &&
            !State::animationActive &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            bool playerMoved = clickLogic(
                event.mouseButton.x,
                event.mouseButton.y);
            if (playerMoved)
            {
                if (State::aiActive)
                {
                    State::aiMoveQueued = true;
                }
            }
        }
        return true;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.control && event.key.code == sf::Keyboard::Z)
        {
            undoLastMove();
            return true;
        }
    }
    return false;
}

// Handle click logic
bool Utility::clickLogic(int x, int y)
{
    Types::Coord coord = calculateSquare(x, y);
    std::cout << coord.x
              << ", "
              << coord.y
              << " | "
              << chessboard.getPiece(coord).toString()
              << std::endl;
    const char player = (State::turns % 2 == 0) ? 'b' : 'w';
    std::string selected = chessboard.getPiece(coord).toString();

    // draw by kings enterning the fortress
    // (click logic takes place outside of the board)
    if (State::selectedPiece == "wKa" &&
        (State::selectedSquare == Types::Coord{0, 0} ||
         State::selectedSquare == Types::Coord{0, 1} ||
         State::selectedSquare == Types::Coord{0, 2}))
    {
        if (coord == Types::Coord{-1, 1})
        {
            State::winner = 'd';
            State::gameOver = true;
            std::cout << "Game ended in a draw" << std::endl;
            return false;
        }
    }
    if (State::selectedPiece == "bKa" &&
        (State::selectedSquare == Types::Coord{10, 9} ||
         State::selectedSquare == Types::Coord{10, 8} ||
         State::selectedSquare == Types::Coord{10, 7}))
    {
        if (coord == Types::Coord{11, 8})
        {
            State::winner = 'd';
            State::gameOver = true;
            std::cout << "Game ended in a draw" << std::endl;
            return false;
        }
    }

    // if click is within the board it is handled here
    if (clickInBoard(x, y))
    {
        if (State::isPieceSelected)
        {
            for (const auto &move : State::moveList)
            {
                if (coord == move)
                {
                    handlePieceMovement(
                        State::selectedPiece.toString(),
                        State::selectedSquare,
                        move,
                        player);
                    // Exit the function after handling the move
                    return true;
                }
            }
        }

        if (State::selectedSquare == coord || selected == "---")
        {
            toggleSelection();
        }
        else if (selected[0] == player)
        {
            handlePieceSelection(coord, player);
        }
    }
    return false;
}

void Utility::playMoveSound()
{
    if (moveSound.getStatus() != sf::Sound::Playing)
    {
        moveSound.play();
    }
}

void Utility::playCaptureSound()
{
    if (captureSound.getStatus() != sf::Sound::Playing)
    {
        captureSound.play();
    }
}

// Handle piece movement
void Utility::handlePieceMovement(
    const std::string &selectedPiece,
    const Types::Coord &selectedSquare,
    const Types::Coord &move,
    const char &player)
{
    render->startAnimation(selectedPiece, selectedSquare, move, 0.5f);
    std::string target = chessboard.getPiece(move).toString();
    chessboard.setCell(selectedSquare, "---");
    chessboard.setCell(move, selectedPiece);
    if (target != "---")
    {
        // Play capture sound
        playCaptureSound();
        if (player == 'w')
        {
            State::blackPiecesCaptured.push_back(target);
        }
        else
        {
            State::whitePiecesCaptured.push_back(target);
        }
    }
    else
    {
        // Play move sound
        playMoveSound();
    }

    Utility::updateGameState(move, target, *gameLogic);

    char enemy = (player == 'w') ? 'b' : 'w';
    gameLogic->promotePawns(player);
    // Check for pawn forks (unique to Tamerlane Chess)
    gameLogic->checkPawnForks(enemy);
    // determine if a draw is possible next turn
    State::drawPossible = gameLogic->canDraw(enemy);
    bool gameOver = checkVictoryCondition(*gameLogic, player, enemy);
    if (gameOver)
    {
        State::gameOver = true;
        std::cout << "Game over. Winner: " << State::winner << std::endl;
    }
}

// Handle piece selection
void Utility::handlePieceSelection(
    const Types::Coord &coord,
    const char &player)
{
    State::selectedSquare = coord;
    State::selectedPiece = chessboard.getPiece(State::selectedSquare);
    std::vector<Types::Coord> possibleMoves = gameLogic->getMoves(
        State::selectedSquare,
        State::selectedPiece,
        player,
        State::alt);
    State::moveList = gameLogic->filterLegalMoves(
        possibleMoves,
        State::selectedSquare,
        State::selectedPiece,
        player,
        State::alt);
    State::isPieceSelected = true;
}

void Utility::handleAiVsAi()
{
    float aiVsAiMoveDelay = 0.1f;
    if (State::aiVsAiMode &&
        !State::animationActive &&
        State::winner == '-' &&
        State::aiVsAiClock.getElapsedTime().asSeconds() >= aiVsAiMoveDelay)
    {
        char aiPlayer = (State::turns % 2 == 0) ? 'b' : 'w';
        Types::Turn aiMove = ai.minMax(
            aiPlayer,
            State::turns,
            State::alt,
            State::aiDifficulty,
            -std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity());
        handlePieceMovement(
            aiMove.pieceMoved.toString(),
            aiMove.initialSquare,
            aiMove.finalSquare,
            aiPlayer);
        State::aiVsAiClock.restart();
    }
}

void Utility::handleMoves()
{
    // Update animations
    render->updateAnimations();

    // Process AI move if queued and animation is finished
    if (State::aiMoveQueued && !State::animationActive && State::winner == '-')
    {
        char aiPlayer = (State::turns % 2 == 0) ? 'b' : 'w';
        Types::Turn aiMove = ai.minMax(
            aiPlayer,
            State::turns,
            State::alt,
            State::aiDifficulty,
            -std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity());
        handlePieceMovement(
            aiMove.pieceMoved.toString(),
            aiMove.initialSquare,
            aiMove.finalSquare,
            aiPlayer);
        State::aiMoveQueued = false;
    }

    // Handle AI vs AI gameplay
    handleAiVsAi();
}
