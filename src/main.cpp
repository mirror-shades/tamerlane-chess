// Include necessary headers
#include "include/gameLogic.h"
#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>

// Constants for the Tamerlane Chess board
const int rows = 10;
const int cols = 11;
const int squareSize = 75;

// Global variables for game state
GameLogic gameLogic;
Utility utility;
int turns = 1;
char winner = '-';
bool isPieceSelected = false;
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
sf::Color colourMove = sf::Color(0xFBFF1255);

// Textures and sprites for chess pieces
std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

// Draw button
sf::RectangleShape drawButton(sf::Vector2f(squareSize, squareSize));
sf::Text drawButtonText;
sf::Font font;

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
        }
    }
}

// Highlight selected square and possible moves
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

// Draw the draw button if possible
void drawDrawButton(sf::RenderWindow &window)
{
    drawButton.setPosition(0, 0);
    drawButton.setFillColor(sf::Color::Green);
    window.draw(drawButton);
    drawButtonText.setFont(font);
    drawButtonText.setString("Draw");
    drawButtonText.setCharacterSize(24);
    drawButtonText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = drawButtonText.getLocalBounds();
    drawButtonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    drawButtonText.setPosition(squareSize * 12.5, squareSize * 9.75);
    window.draw(drawButtonText);
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

    // Draw the draw button if drawPossible is true
    if (drawPossible)
    {
        drawDrawButton(window);
    }
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

// Display win screen
void winScreen(sf::RenderWindow &window)
{
    if (winner != '-')
    {
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
        sprite.setPosition(0, 0);
        window.draw(sprite);
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
void handlePieceMovement(const Types::Coord &move, const char &player)
{
    startAnimation(selectedPiece, selectedSquare, move, 0.5f);
    std::string target = chessboard.getPiece(move);
    chessboard.setCell(selectedSquare, "---");
    chessboard.setCell(move, selectedPiece);

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
void clickLogic(int x, int y)
{
    Types::Coord coord = utility.calculateSquare(x, y);
    std::cout << coord.x << ", " << coord.y << " | " << chessboard.getPiece(coord) << std::endl;
    const char player = (turns % 2 == 0) ? 'b' : 'w';
    const char enemy = (player == 'w') ? 'b' : 'w';
    std::string selected = chessboard.getPiece(coord);

    // initial click position is checked outside of the
    // board to allow for additional functionality
    if (coord.x == -1 && coord.y == 0 && drawPossible)
    {
        winner = 'd';
        gameOver = true;
        std::cout << "Game ended in a draw" << std::endl;
        return;
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
                    handlePieceMovement(move, player);
                    return; // Exit the function after handling the move
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

// Main function
int main()
{
    sf::RenderWindow window(sf::VideoMode(975, 900), "Tamerlane Chess");
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
                if (!gameOver && event.mouseButton.button == sf::Mouse::Left)
                {
                    clickLogic(event.mouseButton.x, event.mouseButton.y);
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
        gameLogic.findAndSetKingPosition(whiteKingPosition, 'w');
        gameLogic.findAndSetKingPosition(blackKingPosition, 'b');
        highlightKing(window, whiteKingPosition, isWhiteKingInCheck);
        highlightKing(window, blackKingPosition, isBlackKingInCheck);

        drawPieces(window, pieceImages);

        winScreen(window); // This line ensures the win screen is drawn if there's a winner.

        window.display();
    }

    return 0;
}