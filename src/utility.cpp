#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include "include/render.h"
#include "include/gameLogic.h"
#include "include/state.h"
#include <iostream>

Render *render;
const int squareSize = 75;

Types::Coord Utility::calculateSquare(int x, int y)
{
    int _x = x / squareSize - 1;
    int _y = y / squareSize;
    return {_x, _y};
}

bool Utility::clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    return !(x < boardOffset || x > boardWidth || y > boardHeight);
}

// Toggle piece selection
void Utility::toggleSelection(const Types::Coord &coord)
{
    State::isPieceSelected = false;
    State::moveList.clear();
    State::selectedSquare = {-1, -1};
}

sf::RectangleShape Utility::createButton(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &fillColor)
{
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(fillColor);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    return button;
}

void Utility::drawButton(sf::RenderWindow &window, const sf::RectangleShape &button, const std::string &text, const sf::Font &font, unsigned int characterSize)
{
    window.draw(button);

    sf::Text buttonText(text, font, characterSize);
    buttonText.setFillColor(sf::Color::Black);

    // Center the text on the button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f);

    window.draw(buttonText);
}

bool Utility::isButtonClicked(const sf::RectangleShape &button, const sf::Vector2i &mousePosition)
{
    return button.getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

// Check if the game has ended (checkmate or stalemate)
bool Utility::checkVictoryCondition(GameLogic &gameLogic, const char &player, const char &enemy)
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
void Utility::updateGameState(const Types::Coord &move, const std::string &target, const char &player, GameLogic &gameLogic)
{
    auto boardState = chessboard.getBoardState();

    State::isWhiteKingInCheck = gameLogic.isKingInCheck('w', boardState, State::alt);
    State::isBlackKingInCheck = gameLogic.isKingInCheck('b', boardState, State::alt);

    Types::Turn newTurn = {
        State::turns,
        State::player,
        State::selectedSquare,
        move,
        State::selectedPiece,
        target};

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

        State::isWhiteKingInCheck = gameLogic->isKingInCheck('w', boardState, State::alt);
        State::isBlackKingInCheck = gameLogic->isKingInCheck('b', boardState, State::alt);

        State::isPieceSelected = false;
        State::moveList.clear();
        State::selectedSquare = {-1, -1};

        std::cout << "Undo move: " << lastTurn.pieceMoved << " from (" << lastTurn.finalSquare.x << ", " << lastTurn.finalSquare.y << ") to (" << lastTurn.initialSquare.x << ", " << lastTurn.initialSquare.y << ")" << std::endl;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    }
}

int Utility::scoreMaterial()
{
    std::map<char, int> pieceValues = {{'p', 1}, {'E', 3}, {'W', 3}, {'A', 3}, {'V', 5}, {'C', 5}, {'M', 5}, {'T', 9}, {'G', 9}, {'R', 5}, {'K', 0}};
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
}
