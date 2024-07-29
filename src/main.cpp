#include "include/logic.h"
#include "include/types.h"
#include "include/globals.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>

Logic logic;

struct Animation
{
    bool isActive = false;
    std::string piece;
    Types::Coord start;
    Types::Coord end;
    sf::Clock clock;
    float duration = 0.25f; // Animation duration in seconds
} animation;

int turns = 1;
const int rows = 10;
const int cols = 11;
const int squareSize = 75;
bool isPieceSelected = false;
std::vector<Types::Coord> moveList;
std::vector<Types::Turn> turnHistory;
Types::Coord selectedSquare = {-1, -1};
std::string selectedPiece;
bool isWhiteKingInCheck = false;
bool isBlackKingInCheck = false;

sf::Color colour1 = sf::Color(0xE5E5E5ff);
sf::Color colour2 = sf::Color(0x26403Cff);
sf::Color colourSelected = sf::Color(0x6290c8ff);
sf::Color colourMove = sf::Color(0xFBFF1255);

std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

std::map<std::string, sf::Sprite>
loadImages()
{
    std::vector<std::string> pieces = {"bKa", "wKa", "bK0", "wK0", "bK1", "wK1", "bAd", "wAd",
                                       "bVi", "wVi", "bGi", "wGi", "bTa", "wTa", "bMo", "wMo",
                                       "bRk", "wRk", "bEl", "wEl", "bCa", "wCa", "bWe", "wWe",
                                       "wp0", "wp1", "wpx", "wpK", "wpA", "wpV", "wpG", "wpT",
                                       "wpM", "wpR", "wpE", "wpC", "wpW", "bp0", "bp1", "bpx",
                                       "bpK", "bpA", "bpV", "bpG", "bpT", "bpM", "bpR", "bpE",
                                       "bpC", "bpW"};

    for (const auto &piece : pieces)
    {
        sf::Texture texture;
        if (!texture.loadFromFile("assets/pieces/" + piece + ".png"))
        {
            std::cerr << "Error loading image: " << piece << ".png" << std::endl;
            continue;
        }

        textures[piece] = texture;

        sf::Sprite sprite;
        sprite.setTexture(textures[piece]);
        sf::Vector2u textureSize = texture.getSize();
        sprite.setScale(
            static_cast<float>(squareSize) / textureSize.x,
            static_cast<float>(squareSize) / textureSize.y);

        images[piece] = sprite;
    }

    return images;
}

Types::Coord calculateSquare(int x, int y)
{
    int _x = x / squareSize - 1;
    int _y = y / squareSize;
    return {_x, _y};
}

bool clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    return !(x < boardOffset || x > boardWidth || y > boardHeight);
}

void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration)
{
    animation.isActive = true;
    animation.piece = piece;
    animation.start = start;
    animation.end = end;
    animation.duration = duration;
    animation.clock.restart();
}

sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t)
{
    return startPos + t * (endPos - startPos);
}

void updateAnimations(float deltaTime)
{
    if (animation.isActive)
    {
        float elapsedTime = animation.clock.getElapsedTime().asSeconds();
        if (elapsedTime >= animation.duration)
        {
            animation.isActive = false;
        }
    }
}

void highlightSquare(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setPosition((selectedSquare.x + 1) * squareSize, selectedSquare.y * squareSize);
    square.setFillColor(colourSelected);
    window.draw(square);

    for (const auto &coord : moveList)
    {
        square.setPosition((coord.x + 1) * squareSize, coord.y * squareSize);
        square.setFillColor(colourMove);
        window.draw(square);
    }
}

void highlightKing(sf::RenderWindow &window, Types::Coord kingPosition, bool isInCheck)
{
    if (isInCheck)
    {
        sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
        square.setPosition((kingPosition.x + 1) * squareSize, kingPosition.y * squareSize);
        square.setFillColor(sf::Color::Red);
        window.draw(square);
    }
}

void findAndSetKingPosition(Types::Coord &kingPosition, const char &player)
{
    auto boardState = chessboard.getBoardState();
    std::string king = (player == 'w') ? "wKa" : "bKa";

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            if (boardState[row][col] == king)
            {
                kingPosition = {col, row};
                return;
            }
        }
    }
}

void drawBoard(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            square.setPosition((col + 1) * squareSize, row * squareSize);
            square.setFillColor((row + col) % 2 != 0 ? colour1 : colour2);
            window.draw(square);
        }
    }

    square.setSize(sf::Vector2f(squareSize, squareSize));

    // Left Fortress
    square.setPosition(0, squareSize);
    square.setFillColor(colour2);
    window.draw(square);

    // Right Fortress
    square.setPosition(squareSize * 12, squareSize * 8);
    square.setFillColor(colour1);
    window.draw(square);
}

void drawPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages)
{
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece != "---")
            {
                sf::Sprite sprite = pieceImages.at(piece);
                if (animation.isActive && animation.piece == piece && animation.end.x == col && animation.end.y == row)
                {
                    float elapsedTime = animation.clock.getElapsedTime().asSeconds();
                    float t = elapsedTime / animation.duration;
                    sf::Vector2f startPos((animation.start.x + 1) * squareSize, animation.start.y * squareSize);
                    sf::Vector2f endPos((animation.end.x + 1) * squareSize, animation.end.y * squareSize);
                    sf::Vector2f currentPos = interpolate(startPos, endPos, t);
                    sprite.setPosition(currentPos);
                }
                else
                {
                    sprite.setPosition((col + 1) * squareSize, row * squareSize);
                }
                window.draw(sprite);
            }
        }
    }
}

sf::Sprite renderBackground(sf::RenderWindow &window, sf::Texture &backgroundTexture)
{
    if (!backgroundTexture.loadFromFile("assets/wood.png"))
    {
        std::cerr << "Error loading background texture" << std::endl;
        throw;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);
    return backgroundSprite;
}

void clickLogic(int x, int y)
{
    Types::Coord coord = calculateSquare(x, y);
    std::cout << coord.x << ", " << coord.y << " | " << chessboard.getPiece(coord) << std::endl;
    auto boardState = chessboard.getBoardState();
    std::string const selected = chessboard.getPiece(coord);
    char player = (turns % 2 == 0) ? 'b' : 'w';

    if (isPieceSelected)
    {
        for (const auto &move : moveList)
        {
            if (coord == move)
            {
                startAnimation(selectedPiece, selectedSquare, move, 0.5f);
                std::string target = chessboard.getPiece(move);
                chessboard.setCell(selectedSquare, "---");
                chessboard.setCell(move, selectedPiece);

                // Update board state after the move
                auto newBoardState = chessboard.getBoardState();
                char enemy = (player == 'w') ? 'b' : 'w';

                // Update king in check status
                isWhiteKingInCheck = logic.isKingInCheck('w');
                isBlackKingInCheck = logic.isKingInCheck('b');

                Types::Turn newTurn = {
                    turns,
                    player,
                    selectedSquare,
                    move,
                    selectedPiece,
                    target};

                turnHistory.push_back(newTurn);
                turns++;
                isPieceSelected = false;
                moveList = {};
                selectedSquare = {-1, -1};
                break;
            }
        }
    }

    if ((selectedSquare == coord) || selected == "---")
    {
        isPieceSelected = false;
        moveList = {};
        selectedSquare = {-1, -1};
    }
    else if (selected[0] == player)
    {
        isPieceSelected = true;
        selectedSquare = {coord.x, coord.y};
        selectedPiece = chessboard.getPiece(selectedSquare);
        moveList = logic.getMoves(selectedSquare, selectedPiece, player);
    }
}

void undoLastMove()
{
    if (!turnHistory.empty())
    {
        Types::Turn lastTurn = turnHistory.back();
        turnHistory.pop_back();
        chessboard.setCell(lastTurn.finalSquare, lastTurn.pieceCaptured);
        chessboard.setCell(lastTurn.initialSquare, lastTurn.pieceMoved);
        turns--;

        // Update king in check status after undo
        isWhiteKingInCheck = logic.isKingInCheck('w');
        isBlackKingInCheck = logic.isKingInCheck('b');

        isPieceSelected = false;
        moveList = {};
        selectedSquare = {-1, -1};

        std::cout << "Undo move: " << lastTurn.pieceMoved << " from (" << lastTurn.finalSquare.x << ", " << lastTurn.finalSquare.y << ") to (" << lastTurn.initialSquare.x << ", " << lastTurn.initialSquare.y << ")" << std::endl;

        animation.isActive = false;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(975, 900), "Chessboard");
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = renderBackground(window, backgroundTexture);
    auto pieceImages = loadImages();
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (clickInBoard(event.mouseButton.x, event.mouseButton.y))
                    {
                        clickLogic(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.control && event.key.code == sf::Keyboard::Z)
                {
                    undoLastMove();
                }
            }
        }

        updateAnimations(deltaTime);

        window.clear(sf::Color::White);
        window.draw(backgroundSprite);
        drawBoard(window);
        highlightSquare(window);

        Types::Coord whiteKingPosition, blackKingPosition;
        findAndSetKingPosition(whiteKingPosition, 'w');
        findAndSetKingPosition(blackKingPosition, 'b');
        highlightKing(window, whiteKingPosition, isWhiteKingInCheck);
        highlightKing(window, blackKingPosition, isBlackKingInCheck);

        drawPieces(window, pieceImages);
        window.display();
    }

    return 0;
}