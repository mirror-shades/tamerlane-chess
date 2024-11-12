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

// Colors for the chess board and piece highlighting
sf::Color colour1 = sf::Color(0xE5E5E5ff);
sf::Color colour2 = sf::Color(0x26403Cff);
sf::Color colourSelected = sf::Color(0x6290c8ff);
sf::Color colourPrevMove = sf::Color(0x6290c855);
sf::Color colourMove = sf::Color(0xFBFF1255);

// Textures and sprites for chess pieces
std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

// Draw exit button
const int exitButtonSize = Chessboard::squareSize / 2;
const sf::Color exitButtonColor = sf::Color::Red;
const sf::Color exitXColor = sf::Color::Black;

// Draw button
sf::RectangleShape drawButton(
    sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
sf::Text drawButtonText;

// ai settings, default difficulty is 2
int aiDifficulty = 2;
sf::RectangleShape slider;
sf::CircleShape sliderHandle;

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
    sf::RectangleShape square(
        sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
    square.setPosition(
        (coord.x + 1) * Chessboard::squareSize,
        coord.y * Chessboard::squareSize);
    square.setFillColor(colourMove);
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
    Types::Coord kingPosition,
    bool isInCheck)
{
    if (isInCheck)
    {
        sf::RectangleShape square(
            sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
        square.setPosition(
            (kingPosition.x + 1) * Chessboard::squareSize,
            kingPosition.y * Chessboard::squareSize);
        square.setFillColor(sf::Color::Red);
        window.draw(square);
    }
}

// Draw the exit button
void Render::drawExitButton(sf::RenderWindow &window)
{
    sf::RectangleShape exitButton(sf::Vector2f(exitButtonSize, exitButtonSize));
    exitButton.setFillColor(exitButtonColor);

    // Position the button in the middle of the top-right square
    float xPos = window.getSize().x - Chessboard::squareSize +
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
        if (exitButton.getGlobalBounds().contains(
                mousePosition.x,
                mousePosition.y))
        {
            utility->exitToMenu();
        }
    }
}

// Draw the Tamerlane Chess board
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
            square.setFillColor((row + col) % 2 != 0 ? colour1 : colour2);
            window.draw(square);
        }
    }

    // Draw Left Fortress (unique to Tamerlane Chess)
    square.setSize(sf::Vector2f(
        Chessboard::squareSize,
        Chessboard::squareSize));
    square.setPosition(0, Chessboard::squareSize);
    square.setFillColor(colour2);
    window.draw(square);

    // Draw Right Fortress (unique to Tamerlane Chess)
    square.setPosition(Chessboard::squareSize * 12, Chessboard::squareSize * 8);
    square.setFillColor(colour1);
    window.draw(square);
}

// Draw chess pieces on the board
void Render::drawPieces(
    sf::RenderWindow &window,
    const std::map<std::string, sf::Sprite> &pieceImages)
{
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState[row][col].toString();
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
                    sprite.setPosition(
                        (col + 1) * Chessboard::squareSize,
                        row * Chessboard::squareSize);
                }
                window.draw(sprite);
            }
        }
    }
}

// Tint screen
void Render::tintScreen(sf::RenderWindow &window)
{
    sf::RectangleShape square(
        sf::Vector2f(Chessboard::squareSize * 13, Chessboard::squareSize * 13));
    square.setPosition(0, 0);
    square.setFillColor(sf::Color(0x00000088));
    window.draw(square);
}

// Display win screen
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

        sf::Font font;
        if (!font.loadFromFile(findAssetsPath("fonts/arial.ttf")))
        {
            std::cerr << "Error loading font" << std::endl;
            return;
        }

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
        Utility::drawButton(window, menuButton, "Menu", font, 24);
        Utility::drawButton(window, analysisButton, "Analysis", font, 24);

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

// Render background
void Render::drawBackground(sf::RenderWindow &window)
{
    // set background sprite from State::backgroundSprite
    window.draw(State::backgroundSprite);
}

// Load chess piece images
std::map<std::string, sf::Sprite> Render::loadImages(sf::RenderWindow &window)
{
    // Load background texture (now using global texture variable)
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
    State::backgroundSprite.setScale(scaleX, scaleY);

    State::backgroundSprite.setTexture(State::backgroundTexture);

    // List of all piece types in Tamerlane Chess
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

void Render::drawSlider(
    sf::RenderWindow &window,
    sf::Font &font)
{
    // Slider background
    slider.setSize(sf::Vector2f(200, 5));
    slider.setFillColor(sf::Color(150, 150, 150));
    slider.setPosition(
        (window.getSize().x - slider.getSize().x) / 2,
        window.getSize().y / 2 + 25);
    window.draw(slider);

    // Slider handle
    sliderHandle.setRadius(10);
    sliderHandle.setFillColor(sf::Color::White);
    sliderHandle.setOutlineThickness(2);
    sliderHandle.setOutlineColor(sf::Color::Black);
    float handleX = slider.getPosition().x + (aiDifficulty - 1) *
                                                 (slider.getSize().x / 9);
    sliderHandle.setPosition(
        handleX - sliderHandle.getRadius(),
        slider.getPosition().y -
            sliderHandle.getRadius() +
            slider.getSize().y / 2);
    window.draw(sliderHandle);

    // Difficulty text
    sf::Text difficultyText;
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(20);
    difficultyText.setFillColor(sf::Color::White);
    std::stringstream ss;
    ss << "AI Difficulty: " << aiDifficulty;
    difficultyText.setString(ss.str());
    difficultyText.setPosition(
        (window.getSize().x - difficultyText.getLocalBounds().width) / 2,
        slider.getPosition().y - 40);
    window.draw(difficultyText);
}

// Menu screen
void Render::drawMenuScreen(sf::RenderWindow &window)
{
    if (State::state == State::GameState::Menu ||
        State::state == State::GameState::AIOptions)
    {
        tintScreen(window);
        sf::Texture titleTexture;

        if (titleTexture.loadFromFile(findAssetsPath("images/title.png")))
        {
            sf::Sprite titleSprite(titleTexture);

            // Disable smoothing to prevent tearing
            titleTexture.setSmooth(false);

            // Calculate the scale to fit the window width
            float scale = window.getSize().x /
                          static_cast<float>(titleTexture.getSize().x);
            titleSprite.setScale(scale, scale);

            // Center the sprite horizontally and position
            // at 1/4 of window height
            // No need to calculate xPos, as we're scaling to fit width
            float xPos = 0;
            float yPos = (window.getSize().y -
                          titleTexture.getSize().y * scale) /
                         4.0f;

            titleSprite.setPosition(xPos, yPos);

            // Use integer rounding for position to avoid subpixel rendering
            sf::Vector2f position = titleSprite.getPosition();
            titleSprite.setPosition(
                static_cast<int>(position.x + 0.5f),
                static_cast<int>(position.y + 0.5f));

            window.draw(titleSprite);
        }
        else
        {
            std::cerr << "Failed to load title.png" << std::endl;
        }

        // Variables to track button states
        static bool isPlayAsWhiteHighlighted = true;
        static bool isPlayAsBlackHighlighted = false;
        static bool isMascHighlighted = true;
        static bool isFemHighlighted = false;
        static bool isThirdHighlighted = false;
        // Track previous mouse button state
        static bool wasMousePressed = false;

        // Create buttons
        sf::RectangleShape aiButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x - 500) / 2 - 75,
                         window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape aiVsAiButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100,
                         window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape pvpButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 125,
                         window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape masc = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x - 500) / 2 - 75,
                         window.getSize().y / 2 - 25),
            isMascHighlighted ? colourSelected : sf::Color::White);

        sf::RectangleShape fem = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100,
                         window.getSize().y / 2 - 25),
            isFemHighlighted ? colourSelected : sf::Color::White);

        sf::RectangleShape third = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 125,
                         window.getSize().y / 2 - 25),
            isThirdHighlighted ? colourSelected : sf::Color::White);

        sf::RectangleShape blitz = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100,
                         window.getSize().y / 2 + 50),
            State::alt ? colourSelected : sf::Color::White);

        sf::RectangleShape aiBlackButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 50,
                         window.getSize().y / 2 - 100),
            isPlayAsBlackHighlighted ? colourSelected : sf::Color::White);

        sf::RectangleShape aiWhiteButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 250,
                         window.getSize().y / 2 - 100),
            isPlayAsWhiteHighlighted ? colourSelected : sf::Color::White);

        sf::RectangleShape aiPlayButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100,
                         window.getSize().y / 2 + 75),
            sf::Color::White);

        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100,
                         window.getSize().y / 2 + 150),
            sf::Color::White);

        // Create button texts
        sf::Font font;
        if (!font.loadFromFile(findAssetsPath("fonts/arial.ttf")))
        {
            std::cerr << "Failed to load font" << std::endl;
        }

        // Draw buttons and texts
        if (State::state == State::GameState::Menu)
        {
            Utility::drawButton(window,
                                pvpButton,
                                "Player vs Player",
                                font,
                                20);
            Utility::drawButton(window,
                                aiButton,
                                "Player vs AI",
                                font,
                                20);
            Utility::drawButton(window,
                                aiVsAiButton,
                                "AI vs AI",
                                font,
                                20);
            Utility::drawButton(window,
                                masc,
                                "Masc",
                                font,
                                20);
            Utility::drawButton(window,
                                fem,
                                "Fem",
                                font,
                                20);
            Utility::drawButton(window,
                                third,
                                "Third",
                                font,
                                20);
            Utility::drawButton(window,
                                blitz,
                                "Blitz",
                                font,
                                20);
        }
        if (State::state == State::GameState::AIOptions)
        {
            Utility::drawButton(window,
                                aiWhiteButton,
                                "Player as white",
                                font,
                                20);
            Utility::drawButton(window,
                                aiBlackButton,
                                "Player as black",
                                font,
                                20);
            Utility::drawButton(window,
                                aiPlayButton,
                                "Play",
                                font,
                                20);
            Utility::drawButton(window,
                                backButton,
                                "Back",
                                font,
                                20);
            // Draw the slider
            drawSlider(window, font);
        }
        // Handle button clicks
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (mousePressed && !wasMousePressed &&
            State::state == State::GameState::Menu)
        {
            if (Utility::isButtonClicked(pvpButton, mousePosition))
            {
                State::state = State::GameState::Game;
                State::aiActive = false;
            }
            else if (Utility::isButtonClicked(aiButton, mousePosition))
            {
                State::state = State::GameState::AIOptions;
            }
            else if (Utility::isButtonClicked(masc, mousePosition))
            {
                isMascHighlighted = true;
                isFemHighlighted = false;
                isThirdHighlighted = false;
                chessboard.setMasculineBoard();
            }
            else if (Utility::isButtonClicked(fem, mousePosition))
            {
                isMascHighlighted = false;
                isFemHighlighted = true;
                isThirdHighlighted = false;
                chessboard.setFeminineBoard();
            }
            else if (Utility::isButtonClicked(third, mousePosition))
            {
                isMascHighlighted = false;
                isFemHighlighted = false;
                isThirdHighlighted = true;
            }
            else if (Utility::isButtonClicked(aiVsAiButton, mousePosition))
            {
                State::state = State::GameState::Game;
                State::aiVsAiMode = true;
                State::aiVsAiClock.restart();
            }
            else if (Utility::isButtonClicked(blitz, mousePosition))
            {
                State::alt = !State::alt;
            }
        }
        if (mousePressed && !wasMousePressed &&
            State::state == State::GameState::AIOptions)
        {
            if (Utility::isButtonClicked(aiWhiteButton, mousePosition))
            {
                isPlayAsWhiteHighlighted = true;
                isPlayAsBlackHighlighted = false;
            }
            else if (Utility::isButtonClicked(aiBlackButton, mousePosition))
            {
                isPlayAsWhiteHighlighted = false;
                isPlayAsBlackHighlighted = true;
            }
            else if (Utility::isButtonClicked(aiPlayButton, mousePosition))
            {
                State::state = State::GameState::Game;
                State::aiActive = true;
                if (isPlayAsBlackHighlighted)
                {
                    State::aiMoveQueued = true;
                }
            }
            else if (Utility::isButtonClicked(backButton, mousePosition))
            {
                State::state = State::GameState::Menu;
            }
            if (slider.getGlobalBounds().contains(
                    mousePosition.x, mousePosition.y))
            {
                float newDifficulty = (mousePosition.x -
                                       slider.getPosition().x) /
                                          slider.getSize().x * 9 +
                                      1;
                aiDifficulty = std::max(
                    1,
                    std::min(
                        10,
                        static_cast<int>(newDifficulty)));
            }
        }

        wasMousePressed = mousePressed;
    }
}

// Highlight the previous move
void Render::highlightPreviousMove(sf::RenderWindow &window)
{
    if (!State::turnHistory.empty())
    {
        Types::Turn lastTurn = State::turnHistory.back();

        sf::RectangleShape initialSquare(
            sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));
        sf::RectangleShape finalSquare(
            sf::Vector2f(Chessboard::squareSize, Chessboard::squareSize));

        initialSquare.setPosition(
            (lastTurn.initialSquare.x + 1) * Chessboard::squareSize,
            lastTurn.initialSquare.y * Chessboard::squareSize);
        finalSquare.setPosition(
            (lastTurn.finalSquare.x + 1) * Chessboard::squareSize,
            lastTurn.finalSquare.y * Chessboard::squareSize);

        initialSquare.setFillColor(colourPrevMove);
        finalSquare.setFillColor(colourPrevMove);

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
            sprite.setPosition(window.getSize().x - 950 + spacew,
                               window.getSize().y - 80);
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
            sprite.setPosition(window.getSize().x - 950 + spaceb,
                               window.getSize().y - 150);
            window.draw(sprite);
        }
    }

    // Draw score
    int score = utility->scoreMaterial();
    sf::Font font;
    if (!font.loadFromFile(findAssetsPath("fonts/arial.ttf")))
    {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(34);
    text.setFillColor(sf::Color::White);
    text.setString(score > 0 ? "+" +
                                   std::to_string(score)
                             : std::to_string(score));
    text.setPosition(window.getSize().x - 75,
                     window.getSize().y - 95);
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

// rename and rethink this function, it doesn't fit here
void Render::renderGameElements(sf::RenderWindow &window)
{
    if (State::state == State::GameState::Game)
    {
        highlightSquares(window);
        highlightPreviousMove(window);
        highlightKings(window);
        drawPieces(window, State::images);
        drawExitButton(window);
        drawCapturedPieces(window, State::images);
        winScreen(window);
    }
}