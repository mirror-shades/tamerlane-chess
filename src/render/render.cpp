// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include "render.h"
#include "chessboard.h"
#include "gameLogic.h"
#include "types.h"
#include "globals.h"
#include "utility.h"
#include "ai.h"
#include "state.h"
#include <SFML/Graphics.hpp>

// Textures and sprites for chess pieces
std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

// Draw exit button
const int exitButtonSize = Chessboard::squareSize / 2;
const sf::Color exitButtonColor = sf::Color::Red;
const sf::Color exitXColor = sf::Color::Black;

// Draw button
sf::RectangleShape drawButton(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
sf::Text drawButtonText;

// Structure for piece movement animation
struct Animation
{
    bool isActive = false;
    std::string piece;
    Types::Coord start;
    Types::Coord end;
    sf::Clock clock;
    float duration = 0.25f;
} animation;

// Find the assets path
std::string Render::findAssetsPath(const std::string &filename)
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path pathParallel = currentPath / "assets" / filename;
    std::filesystem::path pathOneLevelAbove =
        currentPath.parent_path() / "assets" / filename;

    if (std::filesystem::exists(pathParallel))
    {
        return pathParallel.string();
    }
    else if (std::filesystem::exists(pathOneLevelAbove))
    {
        return pathOneLevelAbove.string();
    }

    return "";
}

// Initialize piece movement animation
void Render::startAnimation(
    std::string piece,
    Types::Coord start,
    Types::Coord end,
    float duration)
{
    animation.isActive = true;
    State::animationActive = true;
    animation.piece = piece;
    animation.start = start;
    animation.end = end;
    animation.duration = duration;
    animation.clock.restart();
}

// Calculate intermediate position for smooth animation
sf::Vector2f Render::interpolate(
    sf::Vector2f startPos,
    sf::Vector2f endPos,
    float t)
{
    return startPos + t * (endPos - startPos);
}

// Update animation state
void Render::updateAnimations()
{
    if (animation.isActive)
    {
        float elapsedTime = animation.clock.getElapsedTime().asSeconds();
        if (elapsedTime >= animation.duration)
        {
            animation.isActive = false;
            State::animationActive = false;
        }
    }
}

// Highlight a single square
void Render::highlightSquare(
    sf::RenderWindow &window,
    const Types::Coord &coord)
{
    sf::RectangleShape square(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
    square.setPosition((coord.x + 1) * Chessboard::squareSize, coord.y * Chessboard::squareSize);
    square.setFillColor(State::colourMove);
    window.draw(square);
}

// Highlight selected square and possible moves
void Render::highlightSquares(sf::RenderWindow &window)
{
    highlightSquare(window, State::selectedSquare);

    if (State::selectedPiece == "wKa" &&
        (State::selectedSquare == Types::Coord{0, 0} ||
         State::selectedSquare == Types::Coord{0, 1} ||
         State::selectedSquare == Types::Coord{0, 2}))
    {
        highlightSquare(window, {-1, 1});
    }
    if (State::selectedPiece == "bKa" &&
        (State::selectedSquare == Types::Coord{10, 9} ||
         State::selectedSquare == Types::Coord{10, 8} ||
         State::selectedSquare == Types::Coord{10, 7}))
    {
        highlightSquare(window, {11, 8});
    }

    for (const auto &coord : State::moveList)
    {
        highlightSquare(window, coord);
    }
}

// Highlight king if in check
void Render::highlightKing(
    sf::RenderWindow &window,
    Types::Coord kingPos,
    bool isInCheck)
{
    if (isInCheck)
    {
        sf::RectangleShape square(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
        square.setPosition((kingPos.x + 1) * Chessboard::squareSize, kingPos.y * Chessboard::squareSize);
        square.setFillColor(sf::Color::Red);
        window.draw(square);
    }
}

void Render::drawExitButton(sf::RenderWindow &window)
{
    sf::RectangleShape exitButton(sf::Vector2f(exitButtonSize, exitButtonSize));
    exitButton.setFillColor(exitButtonColor);

    // Position the button in the middle of the top-right square
    float xPos = window.getSize().x -
                 Chessboard::squareSize +
                 (Chessboard::squareSize - exitButtonSize) / 2.0f;
    float yPos = (Chessboard::squareSize - exitButtonSize) / 2.0f;
    exitButton.setPosition(xPos, yPos);

    sf::RectangleShape xLine1(sf::Vector2f(exitButtonSize * 0.7f, 2));
    sf::RectangleShape xLine2(sf::Vector2f(exitButtonSize * 0.7f, 2));
    xLine1.setFillColor(exitXColor);
    xLine2.setFillColor(exitXColor);
    xLine1.setOrigin(xLine1.getSize() / 2.f);
    xLine2.setOrigin(xLine2.getSize() / 2.f);
    xLine1.setPosition(xPos + exitButtonSize / 2, yPos + exitButtonSize / 2);
    xLine2.setPosition(xPos + exitButtonSize / 2, yPos + exitButtonSize / 2);
    xLine1.setRotation(45);
    xLine2.setRotation(-45);

    window.draw(exitButton);
    window.draw(xLine1);
    window.draw(xLine2);

    // Check if the exit button is clicked
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (exitButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            utility->exitToMenu();
        }
    }
}

void Render::drawBoard(sf::RenderWindow &window)
{
    sf::RectangleShape square(
        sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));

    // Draw main 10x11 board
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            square.setPosition(
                (col + 1) * Chessboard::squareSize,
                row * Chessboard::squareSize);
            square.setFillColor((row + col) % 2 != 0 ? State::colour1 : State::colour2);
            window.draw(square);
        }
    }

    // Draw Left Fortresses
    square.setSize(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
    square.setPosition(0, Chessboard::squareSize);
    square.setFillColor(State::colour2);
    window.draw(square);
    square.setPosition(Chessboard::squareSize * 12, Chessboard::squareSize * 8);
    square.setFillColor(State::colour1);
    window.draw(square);
}

void Render::drawPieces(
    sf::RenderWindow &window,
    const std::map<std::string, sf::Sprite> &pieceImages)
{
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState.board[row][col].toString();
            if (piece != "---")
            {
                sf::Sprite sprite = pieceImages.at(piece);
                if (animation.isActive &&
                    animation.piece == piece &&
                    animation.end.x == col &&
                    animation.end.y == row)
                {
                    float elapsedTime =
                        animation.clock.getElapsedTime().asSeconds();
                    float t = elapsedTime / animation.duration;
                    sf::Vector2f startPos(
                        (animation.start.x + 1) * Chessboard::squareSize,
                        animation.start.y * Chessboard::squareSize);
                    sf::Vector2f endPos(
                        (animation.end.x + 1) * Chessboard::squareSize,
                        animation.end.y * Chessboard::squareSize);
                    sf::Vector2f currentPos = interpolate(startPos, endPos, t);
                    sprite.setPosition(currentPos);
                }
                else
                {
                    sprite.setPosition((col + 1) * Chessboard::squareSize, row * Chessboard::squareSize);
                }
                window.draw(sprite);
            }
        }
    }
}

void Render::drawGrid(sf::RenderWindow &window, int px)
{
    unsigned int windowWidth = window.getSize().x;
    unsigned int windowHeight = window.getSize().y;
    
    // Draw vertical lines every px pixels
    for (int x = 0; x <= static_cast<int>(windowWidth); x += px)
    {
        sf::RectangleShape line(sf::Vector2f(1, windowHeight));
        line.setFillColor(sf::Color::White);
        line.setPosition(static_cast<float>(x), 0);
        window.draw(line);
    }
    
    // Draw horizontal lines every px pixels
    for (int y = 0; y <= static_cast<int>(windowHeight); y += px)
    {
        sf::RectangleShape line(sf::Vector2f(windowWidth, 1));
        line.setFillColor(sf::Color::White);
        line.setPosition(0, static_cast<float>(y));
        window.draw(line);
    }
}

void Render::tintScreen(sf::RenderWindow &window)
{
    sf::RectangleShape square(
        sf::Vector2f(Chessboard::squareSize * 13, Chessboard::squareSize * 13));
    square.setPosition(0, 0);
    square.setFillColor(sf::Color(0x00000088));
    window.draw(square);
}

void Render::winScreen(sf::RenderWindow &window)
{
    if (State::winner != '-')
    {
        tintScreen(window);
        sf::Texture texture;
        std::string assetPath;
        if (State::winner == 'w')
        {
            assetPath = findAssetsPath("images/whiteWin.png");
        }
        else if (State::winner == 'b')
        {
            assetPath = findAssetsPath("images/blackWin.png");
        }
        else if (State::winner == 'd')
        {
            assetPath = findAssetsPath("images/draw.png");
        }

        if (!texture.loadFromFile(assetPath))
        {
            std::cerr << "Error loading win screen: " << assetPath << std::endl;
            return;
        }

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sf::Vector2u textureSize = texture.getSize();
        sprite.setPosition(
            (window.getSize().x - textureSize.x) / 2,
            (window.getSize().y - textureSize.y) / 2 - 75);
        window.draw(sprite);

        // Create menu button
        sf::RectangleShape menuButton = Utility::createButton(
            sf::Vector2f(150, 50),
            sf::Vector2f((window.getSize().x / 2) - 160,
                         (window.getSize().y / 2) + 100),
            sf::Color::White);

        // Create analysis button
        sf::RectangleShape analysisButton = Utility::createButton(
            sf::Vector2f(150, 50),
            sf::Vector2f((window.getSize().x / 2) + 10,
                         (window.getSize().y / 2) + 100),
            sf::Color::White);

        // Draw buttons
        Utility::drawButton(window, menuButton, "Menu", 24);
        Utility::drawButton(window, analysisButton, "Analysis", 24);

        // Check for menu button click
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (Utility::isButtonClicked(menuButton, mousePosition))
            {
                utility->exitToMenu();
            }
            else if (Utility::isButtonClicked(analysisButton, mousePosition))
            {
                // Handle analysis button click
            }
        }

        State::ended = true;
    }
}

void Render::drawBackground(sf::RenderWindow &window, const sf::View &view)
{
    // Get the current view center
    sf::Vector2f viewCenter = view.getCenter();
    
    // Draw background at a fixed world position and size
    // As the view zooms, it naturally shows more/less of the background
    const float maxViewWidth = static_cast<float>(State::WINDOW_WIDTH) / ZOOMED_OUT_SCALE;
    const float maxViewHeight = static_cast<float>(State::WINDOW_HEIGHT) / ZOOMED_OUT_SCALE;
    
    // Position background centered on the board center (same as view center)
    float bgLeft = viewCenter.x - maxViewWidth / 2.0f;
    float bgTop = viewCenter.y - maxViewHeight / 2.0f;
    
    // Get texture dimensions and scale to cover maximum zoomed-out area
    sf::Vector2u textureSize = State::backgroundTexture.getSize();
    float scaleX = maxViewWidth / static_cast<float>(textureSize.x);
    float scaleY = maxViewHeight / static_cast<float>(textureSize.y);
    
    // Draw background sprite at fixed world position
    sf::Sprite bgSprite(State::backgroundTexture);
    bgSprite.setOrigin(0.0f, 0.0f);
    bgSprite.setScale(scaleX, scaleY);
    bgSprite.setPosition(bgLeft, bgTop);
    
    window.draw(bgSprite);
}

std::map<std::string, sf::Sprite> Render::loadImages(sf::RenderWindow &window)
{
    std::string assetPath = findAssetsPath("images/wood.png");
    if (!State::backgroundTexture.loadFromFile(assetPath))
    {
        std::cerr << "Error loading background texture" << std::endl;
        throw;
    }
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = State::backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    State::backgroundSprite.setTexture(State::backgroundTexture);
    State::backgroundSprite.setPosition(0, 0); // Initialize position to origin
    State::backgroundSprite.setScale(scaleX, scaleY); // Initial scale (will be recalculated each frame)

    std::vector<std::string> pieces = {
        "bKa", "wKa", "bK0", "wK0", "bK1", "wK1", "bAd", "wAd",
        "bVi", "wVi", "bGi", "wGi", "bTa", "wTa", "bMo", "wMo",
        "bRk", "wRk", "bEl", "wEl", "bCa", "wCa", "bWe", "wWe",
        "wp0", "wp1", "wp2", "wpx", "wpK", "wpA", "wpV", "wpG", "wpT",
        "wpM", "wpR", "wpE", "wpC", "wpW", "bp0", "bp1", "bp2", "bpx",
        "bpK", "bpA", "bpV", "bpG", "bpT", "bpM", "bpR", "bpE",
        "bpC", "bpW"};

    for (const auto &piece : pieces)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(
                findAssetsPath("images/pieces/" + piece + ".png")))
        {
            std::cerr << "Error loading image: " << piece << ".png"
                      << std::endl;
            continue;
        }

        textures[piece] = texture;

        sf::Sprite sprite;
        sprite.setTexture(textures[piece]);
        sf::Vector2u textureSize = texture.getSize();
        sprite.setScale(
            static_cast<float>(Chessboard::squareSize) / textureSize.x,
            static_cast<float>(Chessboard::squareSize) / textureSize.y);

        images[piece] = sprite;
    }

    return images;
}

void Render::highlightPreviousMove(sf::RenderWindow &window)
{
    if (!State::turnHistory.empty())
    {
        Types::Turn lastTurn = State::turnHistory.back();

        sf::RectangleShape initialSquare(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
        sf::RectangleShape finalSquare(sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));

        initialSquare.setPosition(
            (lastTurn.initialSquare.x + 1) * Chessboard::squareSize,
            lastTurn.initialSquare.y * Chessboard::squareSize);
        finalSquare.setPosition(
            (lastTurn.finalSquare.x + 1) * Chessboard::squareSize,
            lastTurn.finalSquare.y * Chessboard::squareSize);

        initialSquare.setFillColor(State::colourPrevMove);
        finalSquare.setFillColor(State::colourPrevMove);

        window.draw(initialSquare);
        window.draw(finalSquare);
    }
}

void Render::drawCapturedPieces(
    sf::RenderWindow &window,
    const std::map<std::string, sf::Sprite> &pieceImages)
{
    std::vector<int> numListw, numListb;
    std::vector<std::string> sortedListw, sortedListb;
    std::map<char, int> pieceToNum = {
        {'p', 1},
        {'E', 2},
        {'W', 3},
        {'A', 4},
        {'V', 5},
        {'C', 6},
        {'M', 7},
        {'T', 8},
        {'G', 9},
        {'R', 10},
        {'K', 11}};
    std::map<int, std::string> numToPiece = {
        {1, "px"},
        {2, "El"},
        {3, "We"},
        {4, "Ad"},
        {5, "Vi"},
        {6, "Ca"},
        {7, "Mo"},
        {8, "Ta"},
        {9, "Gi"},
        {10, "Rk"},
        {11, "Ka"}};

    // Sort white captured pieces
    for (const auto &piece : State::whitePiecesCaptured)
    {
        numListw.push_back(pieceToNum[piece[1]]);
    }
    std::sort(numListw.begin(), numListw.end());
    for (int num : numListw)
    {
        sortedListw.push_back("w" + numToPiece[num]);
    }

    // Sort black captured pieces
    for (const auto &piece : State::blackPiecesCaptured)
    {
        numListb.push_back(pieceToNum[piece[1]]);
    }
    std::sort(numListb.begin(), numListb.end());
    for (int num : numListb)
    {
        sortedListb.push_back("b" + numToPiece[num]);
    }

    // Determine spacing
    int modw = 50, modb = 50;
    if (sortedListw.size() > 17)
        modw = 40;
    if (sortedListw.size() > 21)
        modw = 35;
    if (sortedListw.size() > 24)
        modw = 31;
    if (sortedListb.size() > 17)
        modb = 40;
    if (sortedListb.size() > 21)
        modb = 35;
    if (sortedListb.size() > 24)
        modb = 31;

    // Draw white captured pieces
    for (size_t i = 0; i < sortedListw.size(); ++i)
    {
        int spacew = i * modw;
        if (pieceImages.find(sortedListw[i]) != pieceImages.end())
        {
            sf::Sprite sprite = pieceImages.at(sortedListw[i]);
            sprite.setPosition(window.getSize().x - 950 + spacew, window.getSize().y - 80);
            window.draw(sprite);
        }
    }

    // Draw black captured pieces
    for (size_t i = 0; i < sortedListb.size(); ++i)
    {
        int spaceb = i * modb;
        if (pieceImages.find(sortedListb[i]) != pieceImages.end())
        {
            sf::Sprite sprite = pieceImages.at(sortedListb[i]);
            sprite.setPosition(window.getSize().x - 950 + spaceb, window.getSize().y - 150);
            window.draw(sprite);
        }
    }

    // Draw score
    int score = utility->scoreMaterial();
    sf::Text text;
    text.setFont(Utility::getFont());
    text.setCharacterSize(34);
    text.setFillColor(sf::Color::White);
    // If score is positive (white is winning), add a + sign
    // If score is negative (black is winning), the sign is already there
    text.setString(score > 0 ? "+" + std::to_string(score)
                             : std::to_string(score));
    text.setPosition(window.getSize().x - 75, window.getSize().y - 95);
    window.draw(text);
}

void Render::highlightKings(sf::RenderWindow &window)
{
    Types::Coord whiteKingPosition, blackKingPosition;
    gameLogic->findAndSetKingPosition(whiteKingPosition, 'w');
    gameLogic->findAndSetKingPosition(blackKingPosition, 'b');
    highlightKing(window, whiteKingPosition, State::isWhiteKingInCheck);
    highlightKing(window, blackKingPosition, State::isBlackKingInCheck);
}

// Camera/zoom system implementation
void Render::updateCamera([[maybe_unused]] sf::RenderWindow &window)
{
    if (State::isZooming)
    {
        float elapsed = zoomClock.getElapsedTime().asSeconds();
        State::zoomProgress = std::min(1.0f, elapsed / ZOOM_TRANSITION_TIME);
        
        // Keep rendering during zoom transition
        State::renderNeeded = true;
        
        if (State::zoomProgress >= 1.0f)
        {
            State::isZooming = false;
            State::zoomProgress = 1.0f;
        }
    }
}

void Render::setZoomLevel(State::ZoomLevel targetZoom)
{
    if (State::currentZoomLevel != targetZoom)
    {
        State::previousZoomLevel = State::currentZoomLevel;
        State::currentZoomLevel = targetZoom;
        State::isZooming = true;
        State::zoomProgress = 0.0f;
        State::renderNeeded = true; // Trigger rendering for zoom transition
        zoomClock.restart();
    }
}

sf::View Render::getCurrentView([[maybe_unused]] sf::RenderWindow &window)
{
    // Board dimensions: 12 squares wide (11 + 1 fortress), 10 squares tall
    // Square size: 75px
    const float boardWidth = 12.0f * Chessboard::squareSize;  // 900px
    const float boardHeight = 10.0f * Chessboard::squareSize; // 750px
    const float boardCenterX = boardWidth / 2.0f + 37.0f;  // 450px + 25px (board appears 25px left)
    const float boardCenterY = boardHeight / 2.0f + 75.0f; // 375px + 50px (board appears 50px up)
    
    sf::View view;
    
    // Calculate smooth interpolation if zooming
    float scale = 1.0f;
    
    if (State::isZooming)
    {
        // Determine start and end scales based on zoom direction
        // Use previousZoomLevel to know where we started from
        float startScale = (State::previousZoomLevel == State::ZoomLevel::ZoomedIn) ? 1.0f : ZOOMED_OUT_SCALE;
        float endScale = (State::currentZoomLevel == State::ZoomLevel::ZoomedIn) ? 1.0f : ZOOMED_OUT_SCALE;
        
        // Smooth interpolation (ease in-out)
        float t = State::zoomProgress;
        t = t * t * (3.0f - 2.0f * t); // Smoothstep
        scale = startScale + (endScale - startScale) * t;
    }
    else
    {
        // Not zooming - use current zoom level
        scale = (State::currentZoomLevel == State::ZoomLevel::ZoomedIn) ? 1.0f : ZOOMED_OUT_SCALE;
    }
    
    // Set view size (smaller size = more zoomed in)
    view.setSize(
        static_cast<float>(State::WINDOW_WIDTH) / scale,
        static_cast<float>(State::WINDOW_HEIGHT) / scale
    );
    view.setCenter(boardCenterX, boardCenterY);
    
    return view;
}
