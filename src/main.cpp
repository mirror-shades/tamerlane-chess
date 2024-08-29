// Include necessary headers
#include "include/gameLogic.h"
#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include "include/ai.h"
#include "include/chessboard.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>

// Constants for the Tamerlane Chess board
const int rows = 10;
const int cols = 11;
const int squareSize = 75;

enum class GameState
{
    Menu,
    AIOptions,
    Game
};
GameState state = GameState::Menu;

// Global variables for game state
GameLogic gameLogic;
Utility utility;
AI ai(chessboard);
int turns = 1;
char winner = '-';
bool aiActive = false;
bool aiMoveQueued = false;
bool isPieceSelected = false;
bool animationInProgress = false;
bool ended = false;
bool drawPossible = false;
bool gameOver = false;
bool alt = false; // Alternate moves for blitz variant
Types::Coord selectedSquare = {-1, -1};
std::string selectedPiece;
bool isWhiteKingInCheck = false;
bool isBlackKingInCheck = false;
std::vector<Types::Coord> moveList;
std::vector<Types::Turn> turnHistory;

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
const int exitButtonSize = squareSize / 2;
const sf::Color exitButtonColor = sf::Color::Red;
const sf::Color exitXColor = sf::Color::Black;

// Draw button
sf::RectangleShape drawButton(sf::Vector2f(squareSize, squareSize));
sf::Text drawButtonText;

// ai settings
int aiDifficulty = 3; // Default difficulty
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

// Check if the game has ended (checkmate or stalemate)
bool checkVictoryCondition(const char &player, const char &enemy)
{
    auto boardState = chessboard.getBoardState();
    bool hasLegalMoves = gameLogic.hasLegalMoves(enemy, alt);
    bool kingInCheck = gameLogic.isKingInCheck(enemy, boardState, alt);

    if (!hasLegalMoves)
    {
        if (kingInCheck)
        {
            winner = player;
            std::cout << player << " has won by checkmate" << std::endl;
        }
        else
        {
            winner = 's';
            std::cout << "The game is a draw by stalemate" << std::endl;
        }
        gameOver = true;
        return true;
    }

    return false;
}

// Initialize piece movement animation
void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration)
{
    animation.isActive = true;
    animationInProgress = true;
    animation.piece = piece;
    animation.start = start;
    animation.end = end;
    animation.duration = duration;
    animation.clock.restart();
}

// Calculate intermediate position for smooth animation
sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t)
{
    return startPos + t * (endPos - startPos);
}

// Update animation state
void updateAnimations(float deltaTime)
{
    if (animation.isActive)
    {
        float elapsedTime = animation.clock.getElapsedTime().asSeconds();
        if (elapsedTime >= animation.duration)
        {
            animation.isActive = false;
            animationInProgress = false;
        }
    }
}

// Highlight a single square
void highlightSquare(sf::RenderWindow &window, const Types::Coord &coord)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setPosition((coord.x + 1) * squareSize, coord.y * squareSize);
    square.setFillColor(colourMove);
    window.draw(square);
}

// Highlight selected square and possible moves
void highlightSquares(sf::RenderWindow &window)
{
    highlightSquare(window, selectedSquare);

    if (selectedPiece == "wKa" && (selectedSquare == Types::Coord{0, 0} || selectedSquare == Types::Coord{0, 1} || selectedSquare == Types::Coord{0, 2}))
    {
        highlightSquare(window, {-1, 1});
    }
    if (selectedPiece == "bKa" && (selectedSquare == Types::Coord{10, 9} || selectedSquare == Types::Coord{10, 8} || selectedSquare == Types::Coord{10, 7}))
    {
        highlightSquare(window, {11, 8});
    }

    for (const auto &coord : moveList)
    {
        highlightSquare(window, coord);
    }
}

// Highlight king if in check
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

// Exit to menu
void exitToMenu()
{
    std::cout << "Exiting game" << std::endl;
    // Reset game state and return to menu
    state = GameState::Menu;
    gameOver = false;
    isPieceSelected = false;
    moveList.clear();
    selectedSquare = {-1, -1};
    chessboard.resetBoard();
    turnHistory.clear();
    turns = 1;
    drawPossible = false;
    isWhiteKingInCheck = false;
    isBlackKingInCheck = false;
    winner = '-';
}

// Draw the exit button
void drawExitButton(sf::RenderWindow &window)
{
    sf::RectangleShape exitButton(sf::Vector2f(exitButtonSize, exitButtonSize));
    exitButton.setFillColor(exitButtonColor);

    // Position the button in the middle of the top-right square
    float xPos = window.getSize().x - squareSize + (squareSize - exitButtonSize) / 2.0f;
    float yPos = (squareSize - exitButtonSize) / 2.0f;
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
            exitToMenu();
        }
    }
}

// Draw the Tamerlane Chess board
void drawBoard(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    // Draw main 10x11 board
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            square.setPosition((col + 1) * squareSize, row * squareSize);
            square.setFillColor((row + col) % 2 != 0 ? colour1 : colour2);
            window.draw(square);
        }
    }

    // Draw Left Fortress (unique to Tamerlane Chess)
    square.setSize(sf::Vector2f(squareSize, squareSize));
    square.setPosition(0, squareSize);
    square.setFillColor(colour2);
    window.draw(square);

    // Draw Right Fortress (unique to Tamerlane Chess)
    square.setPosition(squareSize * 12, squareSize * 8);
    square.setFillColor(colour1);
    window.draw(square);
}

// Draw chess pieces on the board
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

// Tint screen
void tintScreen(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize * 13, squareSize * 13));
    square.setPosition(0, 0);
    square.setFillColor(sf::Color(0x00000088));
    window.draw(square);
}

// Display win screen
void winScreen(sf::RenderWindow &window)
{
    if (winner != '-')
    {
        tintScreen(window);
        sf::Texture texture;
        std::string assetPath;
        if (winner == 'w')
        {
            assetPath = "assets/whiteWin.png";
        }
        else if (winner == 'b')
        {
            assetPath = "assets/blackWin.png";
        }
        else if (winner == 'd')
        {
            assetPath = "assets/draw.png";
        }

        if (!ended) // debugging
        {
            std::cout << "Winner: " << winner << ", Loading asset: " << assetPath << std::endl;
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
        if (!font.loadFromFile("assets/arial.ttf"))
        {
            std::cerr << "Error loading font" << std::endl;
            return;
        }

        // Create menu button
        sf::RectangleShape menuButton = Utility::createButton(
            sf::Vector2f(150, 50),
            sf::Vector2f((window.getSize().x / 2) - 160, (window.getSize().y / 2) + 100),
            sf::Color::White);

        // Create analysis button
        sf::RectangleShape analysisButton = Utility::createButton(
            sf::Vector2f(150, 50),
            sf::Vector2f((window.getSize().x / 2) + 10, (window.getSize().y / 2) + 100),
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
                exitToMenu();
            }
            else if (Utility::isButtonClicked(analysisButton, mousePosition))
            {
                // Handle analysis button click
            }
        }

        ended = true;
    }
}

// Render background
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

// Load chess piece images
std::map<std::string, sf::Sprite> loadImages()
{
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

// Handle piece selection
void handlePieceSelection(const Types::Coord &coord, const char &player)
{
    selectedSquare = coord;
    selectedPiece = chessboard.getPiece(selectedSquare);
    std::vector<Types::Coord> possibleMoves = gameLogic.getMoves(selectedSquare, selectedPiece, player, alt);
    moveList = gameLogic.filterLegalMoves(possibleMoves, selectedSquare, selectedPiece, player, alt);
    isPieceSelected = true;
}

// Update game state after a move
void updateGameState(const Types::Coord &move, const std::string &target, const char &player)
{
    auto boardState = chessboard.getBoardState();

    isWhiteKingInCheck = gameLogic.isKingInCheck('w', boardState, alt);
    isBlackKingInCheck = gameLogic.isKingInCheck('b', boardState, alt);

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
    moveList.clear();
    selectedSquare = {-1, -1};
}

// Toggle piece selection
void toggleSelection(const Types::Coord &coord)
{
    isPieceSelected = false;
    moveList.clear();
    selectedSquare = {-1, -1};
}

// Handle piece movement
void handlePieceMovement(const std::string &_selectedPiece, const Types::Coord &_selectedSquare, const Types::Coord &move, const char &player)
{
    startAnimation(_selectedPiece, _selectedSquare, move, 0.5f);
    std::string target = chessboard.getPiece(move);
    chessboard.setCell(_selectedSquare, "---");
    chessboard.setCell(move, _selectedPiece);

    updateGameState(move, target, player);

    char enemy = (player == 'w') ? 'b' : 'w';
    gameLogic.promotePawns(player);
    // Check for pawn forks (unique to Tamerlane Chess)
    gameLogic.checkPawnForks(enemy);
    // determine if a draw is possible next turn
    drawPossible = gameLogic.canDraw(enemy);
    bool game_over = checkVictoryCondition(player, enemy);
    if (game_over)
    {
        gameOver = true;
        std::cout << "Game over. Winner: " << winner << std::endl;
    }
}

// Handle click logic
bool clickLogic(int x, int y)
{
    Types::Coord coord = utility.calculateSquare(x, y);
    std::cout << coord.x << ", " << coord.y << " | " << chessboard.getPiece(coord) << std::endl;
    const char player = (turns % 2 == 0) ? 'b' : 'w';
    const char enemy = (player == 'w') ? 'b' : 'w';
    std::string selected = chessboard.getPiece(coord);

    // draw by kings enterning the fortress (click logic takes place outside of the board)
    if (selectedPiece == "wKa" && (selectedSquare == Types::Coord{0, 0} || selectedSquare == Types::Coord{0, 1} || selectedSquare == Types::Coord{0, 2}))
    {
        if (coord == Types::Coord{-1, 1})
        {
            winner = 'd';
            gameOver = true;
            std::cout << "Game ended in a draw" << std::endl;
            return false;
        }
    }
    if (selectedPiece == "bKa" && (selectedSquare == Types::Coord{10, 9} || selectedSquare == Types::Coord{10, 8} || selectedSquare == Types::Coord{10, 7}))
    {
        if (coord == Types::Coord{11, 8})
        {
            winner = 'd';
            gameOver = true;
            std::cout << "Game ended in a draw" << std::endl;
            return false;
        }
    }

    // if click is within the board it is handled here
    if (utility.clickInBoard(x, y))
    {
        if (isPieceSelected)
        {
            for (const auto &move : moveList)
            {
                if (coord == move)
                {
                    handlePieceMovement(selectedPiece, selectedSquare, move, player);
                    return true; // Exit the function after handling the move
                }
            }
        }

        if (selectedSquare == coord || selected == "---")
        {
            toggleSelection(coord);
        }
        else if (selected[0] == player)
        {
            handlePieceSelection(coord, player);
        }
    }
    return false;
}

// Undo the last move
void undoLastMove()
{
    if (!turnHistory.empty())
    {
        Types::Turn lastTurn = turnHistory.back();
        turnHistory.pop_back();
        chessboard.setCell(lastTurn.finalSquare, lastTurn.pieceCaptured);
        chessboard.setCell(lastTurn.initialSquare, lastTurn.pieceMoved);
        turns--;
        auto boardState = chessboard.getBoardState();

        isWhiteKingInCheck = gameLogic.isKingInCheck('w', boardState, alt);
        isBlackKingInCheck = gameLogic.isKingInCheck('b', boardState, alt);

        isPieceSelected = false;
        moveList.clear();
        selectedSquare = {-1, -1};

        std::cout << "Undo move: " << lastTurn.pieceMoved << " from (" << lastTurn.finalSquare.x << ", " << lastTurn.finalSquare.y << ") to (" << lastTurn.initialSquare.x << ", " << lastTurn.initialSquare.y << ")" << std::endl;
        animation.isActive = false;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    }
}

// Add this new function to draw the slider
void drawSlider(sf::RenderWindow &window, sf::Font &font)
{
    // Slider background
    slider.setSize(sf::Vector2f(200, 5));
    slider.setFillColor(sf::Color(150, 150, 150));
    slider.setPosition((window.getSize().x - slider.getSize().x) / 2, window.getSize().y / 2 + 150);
    window.draw(slider);

    // Slider handle
    sliderHandle.setRadius(10);
    sliderHandle.setFillColor(sf::Color::White);
    sliderHandle.setOutlineThickness(2);
    sliderHandle.setOutlineColor(sf::Color::Black);
    float handleX = slider.getPosition().x + (aiDifficulty - 1) * (slider.getSize().x / 4);
    sliderHandle.setPosition(handleX - sliderHandle.getRadius(), slider.getPosition().y - sliderHandle.getRadius() + slider.getSize().y / 2);
    window.draw(sliderHandle);

    // Difficulty text
    sf::Text difficultyText;
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(20);
    difficultyText.setFillColor(sf::Color::White);
    std::stringstream ss;
    ss << "AI Difficulty: " << aiDifficulty;
    difficultyText.setString(ss.str());
    difficultyText.setPosition((window.getSize().x - difficultyText.getLocalBounds().width) / 2, slider.getPosition().y - 40);
    window.draw(difficultyText);
}

// Menu screen
void drawMenuScreen(sf::RenderWindow &window)
{
    tintScreen(window);
    sf::Texture titleTexture;

    if (titleTexture.loadFromFile("assets/title.png"))
    {
        sf::Sprite titleSprite(titleTexture);

        // Disable smoothing to prevent tearing
        titleTexture.setSmooth(false);

        // Calculate the scale to fit the window width
        float scale = window.getSize().x / static_cast<float>(titleTexture.getSize().x);
        titleSprite.setScale(scale, scale);

        // Center the sprite horizontally and position it at 1/4 of the window height
        float xPos = 0; // No need to calculate, as we're scaling to fit the width
        float yPos = (window.getSize().y - titleTexture.getSize().y * scale) / 4.0f;

        titleSprite.setPosition(xPos, yPos);

        // Use integer rounding for position to avoid subpixel rendering
        sf::Vector2f position = titleSprite.getPosition();
        titleSprite.setPosition(static_cast<int>(position.x + 0.5f), static_cast<int>(position.y + 0.5f));

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
    static bool wasMousePressed = false; // Track previous mouse button state

    // Create buttons
    sf::RectangleShape pveButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x - 500) / 2, window.getSize().y / 2 - 100),
        sf::Color::White);

    sf::RectangleShape pvpButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 + 50, window.getSize().y / 2 - 100),
        sf::Color::White);

    sf::RectangleShape masc = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x - 500) / 2 - 50, window.getSize().y / 2 - 25),
        isMascHighlighted ? colourSelected : sf::Color::White);

    sf::RectangleShape fem = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 - 25),
        isFemHighlighted ? colourSelected : sf::Color::White);

    sf::RectangleShape third = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x + 200) / 2, window.getSize().y / 2 - 25),
        isThirdHighlighted ? colourSelected : sf::Color::White);

    sf::RectangleShape blitz = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 50),
        alt ? colourSelected : sf::Color::White);

    sf::RectangleShape pveBlackButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 + 50, window.getSize().y / 2 - 100),
        isPlayAsBlackHighlighted ? colourSelected : sf::Color::White);

    sf::RectangleShape pveWhiteButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 250, window.getSize().y / 2 - 100),
        isPlayAsWhiteHighlighted ? colourSelected : sf::Color::White);

    sf::RectangleShape pvePlayButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 75),
        sf::Color::White);

    sf::RectangleShape backButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 150),
        sf::Color::White);

    // Create button texts
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) // Make sure you have this font file
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Draw buttons and texts
    if (state == GameState::Menu)
    {
        Utility::drawButton(window, pvpButton, "Player vs Player", font, 20);
        Utility::drawButton(window, pveButton, "Player vs AI", font, 20);
        Utility::drawButton(window, masc, "Masc", font, 20);
        Utility::drawButton(window, fem, "Fem", font, 20);
        Utility::drawButton(window, third, "Third", font, 20);
        Utility::drawButton(window, blitz, "Blitz", font, 20);
    }
    if (state == GameState::AIOptions)
    {
        Utility::drawButton(window, pveWhiteButton, "Player as white", font, 20);
        Utility::drawButton(window, pveBlackButton, "Player as black", font, 20);
        Utility::drawButton(window, pvePlayButton, "Play", font, 20);
        Utility::drawButton(window, backButton, "Back", font, 20);
    }
    // Handle button clicks
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (mousePressed && !wasMousePressed && state == GameState::Menu)
    {
        if (Utility::isButtonClicked(pvpButton, mousePosition))
        {
            state = GameState::Game;
            aiActive = false;
        }
        else if (Utility::isButtonClicked(pveButton, mousePosition))
        {
            state = GameState::AIOptions;
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
        else if (Utility::isButtonClicked(blitz, mousePosition))
        {
            alt = !alt;
        }
    }
    if (mousePressed && !wasMousePressed && state == GameState::AIOptions)
    {
        if (Utility::isButtonClicked(pveWhiteButton, mousePosition))
        {
            isPlayAsWhiteHighlighted = true;
            isPlayAsBlackHighlighted = false;
        }
        else if (Utility::isButtonClicked(pveBlackButton, mousePosition))
        {
            isPlayAsWhiteHighlighted = false;
            isPlayAsBlackHighlighted = true;
        }
        else if (Utility::isButtonClicked(pvePlayButton, mousePosition))
        {
            state = GameState::Game;
            aiActive = true;
            if (isPlayAsBlackHighlighted)
            {
                aiMoveQueued = true;
            }
        }
        else if (Utility::isButtonClicked(backButton, mousePosition))
        {
            state = GameState::Menu;
        }
    }

    wasMousePressed = mousePressed;
}

// Highlight the previous move
void highlightPreviousMove(sf::RenderWindow &window)
{
    if (!turnHistory.empty())
    {
        Types::Turn lastTurn = turnHistory.back();
        sf::RectangleShape initialSquare(sf::Vector2f(squareSize, squareSize));
        sf::RectangleShape finalSquare(sf::Vector2f(squareSize, squareSize));

        initialSquare.setPosition((lastTurn.initialSquare.x + 1) * squareSize, lastTurn.initialSquare.y * squareSize);
        finalSquare.setPosition((lastTurn.finalSquare.x + 1) * squareSize, lastTurn.finalSquare.y * squareSize);

        initialSquare.setFillColor(colourPrevMove);
        finalSquare.setFillColor(colourPrevMove);

        window.draw(initialSquare);
        window.draw(finalSquare);
    }
}

// Main function
int main()
{
    // Initialize the game window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Tamerlane Chess");

    // Load and set up the background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = renderBackground(window, backgroundTexture);

    // Load chess piece images
    auto pieceImages = loadImages();

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
                if (!gameOver && !animationInProgress && event.mouseButton.button == sf::Mouse::Left)
                {
                    bool playerMoved = clickLogic(event.mouseButton.x, event.mouseButton.y);

                    if (playerMoved && aiActive)
                    {
                        aiMoveQueued = true;
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

        // Update animations
        updateAnimations(deltaTime);

        // Process AI move if queued and animation is finished
        if (aiMoveQueued && !animationInProgress && winner == '-')
        {
            char aiPlayer = (turns % 2 == 0) ? 'b' : 'w';
            Types::Turn aiMove = ai.minMax(gameLogic, aiPlayer, turns, alt, 3,
                                           -std::numeric_limits<float>::infinity(),
                                           std::numeric_limits<float>::infinity());
            handlePieceMovement(aiMove.pieceMoved, aiMove.initialSquare, aiMove.finalSquare, 'b');
            aiMoveQueued = false;
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the background
        window.draw(backgroundSprite);

        // Draw the chess board
        drawBoard(window);

        if (state == GameState::Menu || state == GameState::AIOptions)
        {
            drawMenuScreen(window);
        }

        else
        {
            highlightSquares(window);
            highlightPreviousMove(window);
            Types::Coord whiteKingPosition, blackKingPosition;
            gameLogic.findAndSetKingPosition(whiteKingPosition, 'w');
            gameLogic.findAndSetKingPosition(blackKingPosition, 'b');
            highlightKing(window, whiteKingPosition, isWhiteKingInCheck);
            highlightKing(window, blackKingPosition, isBlackKingInCheck);
            drawPieces(window, pieceImages);
            drawExitButton(window);

            winScreen(window);
        }

        // Display everything that was drawn
        window.display();
    }

    return 0;
}