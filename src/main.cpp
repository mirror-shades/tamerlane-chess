#include "include/logic.h"
#include "include/types.h"
#include "include/globals.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>

Logic logic;

int turns = 1;
const int rows = 10;
const int cols = 11;
const int squareSize = 75;
bool isPieceSelected = false;
std::vector<Types::Coord> moveList;
Types::Coord selectedSquare = {-1, -1};
std::string selectedPiece;

std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

std::map<std::string, sf::Sprite> loadImages()
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

        // Store the texture to keep it alive
        textures[piece] = texture;

        sf::Sprite sprite;
        sprite.setTexture(textures[piece]);

        // Resize the sprite to (sq_size, sq_size)
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
    Types::Coord coord = {_x, _y};
    return coord;
}

bool clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    if (x < boardOffset || x > boardWidth || y > boardHeight)
    {
        return false;
    }
    return true;
}

std::vector<Types::Coord> getMoves(Types::Coord coord, std::string piece, char player)
{
    std::vector<Types::Coord> _moveList = {};
    if (piece[1] == 'p')
    {
        _moveList = logic.getPawnMoves(coord, player);
    }
    return _moveList;
}

void clickLogic(int x, int y)
{
    Types::Coord coord = calculateSquare(x, y);
    std::cout << coord.x << ", " << coord.y << " | " << chessboard.getPiece(coord) << std::endl;
    auto boardState = chessboard.getBoardState();
    std::string const selected = chessboard.getPiece(coord);
    char player = (turns % 2 == 0) ? 'b' : 'w'; // player turn is decided by even/odd (white goes on turn 1)
    // if reclicking on selected square, or on a non-valid square
    if (isPieceSelected)
    {
        for (const auto &move : moveList)
        {
            if (coord == move)
            {
                std::cout << "selected " << selectedSquare.x << ", " << selectedSquare.y << std::endl;
                std::cout << "move " << move.x << ", " << move.y << std::endl;
                Types::Coord selectedOffset = {selectedSquare.x, selectedSquare.y};
                chessboard.setCell(selectedOffset, "---");
                chessboard.setCell(move, selectedPiece);
                isPieceSelected = false;
                moveList = {};
                selectedSquare = {-1, -1};
                break;
            }
        }
    }
    if ((selectedSquare.x == coord.x + 1 && selectedSquare.y == coord.y) || selected == "---")
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
        moveList = getMoves(coord, selected, player);
        std::cout << selectedPiece << std::endl;
    }
}

void highlightSquare(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    // Position the square
    square.setPosition((selectedSquare.x + 1) * squareSize, selectedSquare.y * squareSize);

    // highlight
    square.setFillColor(sf::Color(250, 250, 210, 200));
    // Draw the square
    window.draw(square);

    for (const auto &coord : moveList)
    { // Position the square
        square.setPosition((coord.x + 1) * squareSize, coord.y * squareSize);

        // highlight
        square.setFillColor(sf::Color(250, 250, 210, 200));
        // Draw the square
        window.draw(square);
    }
}

void drawBoard(sf::RenderWindow &window)
{
    // Draw the chessboard
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

            // Position the square
            square.setPosition((col + 1) * squareSize, row * squareSize);

            // Alternate colors
            if ((row + col) % 2 == 0)
            {
                square.setFillColor(sf::Color::Black);
            }
            else
            {
                square.setFillColor(sf::Color(165, 42, 42));
            }

            // Draw the square
            window.draw(square);
        }
    }

    // Draw the fortresses (squares on the sides)
    sf::RectangleShape lFortress(sf::Vector2f(squareSize, squareSize));
    lFortress.setPosition(0, squareSize);
    lFortress.setFillColor(sf::Color::Black);
    window.draw(lFortress);

    sf::RectangleShape rFortress(sf::Vector2f(squareSize, squareSize));
    rFortress.setPosition(squareSize * 12, squareSize * 8);
    rFortress.setFillColor(sf::Color(165, 42, 42));
    window.draw(rFortress);
}

void drawPieces(sf::RenderWindow &window, auto images)
{
    auto boardState = chessboard.getBoardState();
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece != "---")
            {
                sf::Sprite sprite = images[piece];
                sprite.setPosition((col + 1) * squareSize, row * squareSize);
                window.draw(sprite);
            }
        }
    }
}

sf::Sprite renderBackground(sf::RenderWindow &window, sf::Texture &backgroundTexture)
{
    // Load background texture from a file
    if (!backgroundTexture.loadFromFile("assets/wood.png"))
    {
        std::cerr << "Error loading background texture" << std::endl;
        throw;
    }

    // Create a sprite using the texture
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Get the size of the window and texture
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Calculate scale factors for x and y directions
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    // Apply the scale to the sprite
    backgroundSprite.setScale(scaleX, scaleY);
    return backgroundSprite;
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Chessboard");

    // Declare the texture outside of the renderBackground function for persistence
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = renderBackground(window, backgroundTexture);

    // Load piece images
    auto pieceImages = loadImages();

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse button pressed event
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bool boardClick = clickInBoard(event.mouseButton.x, event.mouseButton.y);
                    if (boardClick)
                    {
                        clickLogic(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }
        }

        // Clear the window with white color
        window.clear(sf::Color::White);
        // Draw the background
        window.draw(backgroundSprite);
        // Draw the chessboard
        drawBoard(window);
        // highlight selected piece
        highlightSquare(window);
        // Draw the pieces
        drawPieces(window, pieceImages);

        // End the current frame
        window.display();
    }

    return 0;
}