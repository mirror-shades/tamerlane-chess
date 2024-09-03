#pragma once
#include <SFML/Graphics.hpp>
#include "types.h"

class Game
{
public:
    enum class GameState
    {
        Menu,
        AIOptions,
        Game
    };

    static GameState state;
    static bool gameOver;
    static bool animationInProgress;
    static bool aiActive;
    static bool aiMoveQueued;
    static bool isWhiteKingInCheck;
    static bool isBlackKingInCheck;
    static bool ended;
    static bool drawPossible;
    static bool isPieceSelected;
    static char winner;
    static int turns;
    static int alt;
    static int aiDifficulty;
    static std::string selectedPiece;
    static Types::Coord selectedSquare;
    static Types::Coord move;
    static char player;
    static std::vector<Types::Coord> moveList;
    static std::vector<std::string> moveHistory;
    std::vector<Types::Turn> turnHistory;

    void handlePieceMovement(const std::string &_selectedPiece, const Types::Coord &_selectedSquare, const Types::Coord &move, const char &player);
    sf::Sprite renderBackground(sf::RenderWindow &window, sf::Texture &backgroundTexture);
    std::map<std::string, sf::Sprite> loadImages();
    bool clickLogic(int x, int y);
    void undoLastMove();
    void updateAnimations(float deltaTime);
    void handleAiVsAi();
    void drawBoard(sf::RenderWindow &window);
    void exitToMenu();
    void drawExitButton(sf::RenderWindow &window);
    void winScreen(sf::RenderWindow &window);
    void drawMenuScreen(sf::RenderWindow &window);
    void highlightSquares(sf::RenderWindow &window);
    void highlightPreviousMove(sf::RenderWindow &window);
    void highlightKing(sf::RenderWindow &window, Types::Coord kingPosition, bool isInCheck);
    void drawPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void drawCapturedPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    bool checkVictoryCondition(const char &player, const char &enemy);
    void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration);
    void handlePieceSelection(const Types::Coord &coord, const char &player);
    void updateGameState(const Types::Coord &move, const std::string &target, const char &player);
    void toggleSelection(const Types::Coord &coord);
    void handleMoves(sf::RenderWindow &window);
    void gameHandler(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    bool clickHandler(sf::Event event, sf::RenderWindow &window);
    void gameFrame(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages, sf::Sprite &backgroundSprite);
};