#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

const int squareSize = 75;
const int circleSize = 25;
const int rows = 10;
const int cols = 11;

std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

std::string chessboard[rows][cols] = {
    {"bEl", "---", "bCa", "---", "bWe", "---", "bWe", "---", "bCa", "---", "bEl"},
    {"bRk", "bMo", "bTa", "bGi", "bVi", "bKa", "bAd", "bGi", "bTa", "bMo", "bRk"},
    {"bpR", "bpM", "bpT", "bpG", "bpV", "bpK", "bpA", "bpE", "bpC", "bpW", "bp0"},
    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    {"---", "---", "---", "---", "---", "---", "---", "---", "---", "---", "---"},
    {"wp0", "wpW", "wpC", "wpE", "wpA", "wpK", "wpV", "wpG", "wpT", "wpM", "wpR"},
    {"wRk", "wMo", "wTa", "wGi", "wAd", "wKa", "wVi", "wGi", "wTa", "wMo", "wRk"},
    {"wEl", "---", "wCa", "---", "wWe", "---", "wWe", "---", "wCa", "---", "wEl"}};

struct Coord
{
    int x;
    int y;
};

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

Coord calculateSquare(int x, int y)
{
    int _x = x / squareSize;
    int _y = y / squareSize;
    Coord coord = {_x - 1, _y};
    return coord;
}

bool clickInBoard(int x, int y)
{
    if (x < 75)
    {
        return false;
    }
    if (x > 900)
    {
        return false;
    }
    if (y > 750)
    {
        return false;
    }
    return true;
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

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string piece = chessboard[row][col];
            if (piece != "---")
            {
                sf::Sprite sprite = images[piece];
                sprite.setPosition((col + 1) * squareSize, row * squareSize);
                window.draw(sprite);
            }
        }
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Chessboard");

    // Load background texture from a file
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/wood.png"))
    {
        std::cerr << "Error loading background texture" << std::endl;
        return -1;
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
                    std::cout << "Left mouse button pressed at ("
                              << event.mouseButton.x << ", "
                              << event.mouseButton.y << ")\n";
                    bool boardClick = clickInBoard(event.mouseButton.x, event.mouseButton.y);
                    if (boardClick)
                    {
                        Coord coord = calculateSquare(event.mouseButton.x, event.mouseButton.y);
                        std::cout << coord.x << ", " << coord.y << std::endl;
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
        // Draw the pieces
        drawPieces(window, pieceImages);

        // End the current frame
        window.display();
    }

    return 0;
}