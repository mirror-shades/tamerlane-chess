#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

const int squareSize = 75;
const int circleSize = 25;
const int rows = 10;
const int cols = 11;

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

void drawPieces(sf::RenderWindow &window)
{
    // Draw the peices
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            sf::CircleShape circle;

            circle.setRadius(circleSize);

            // Position the peice
            circle.setPosition(((col + 1) * squareSize) + circleSize / 2, (row * squareSize) + circleSize / 2);

            // Alternate colors
            if (chessboard[row][col] != "---")
            {
                circle.setFillColor(sf::Color::Blue);
                // Draw the square
                window.draw(circle);
            }
        }
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Chessboard");

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
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    std::cout << "Right mouse button pressed at ("
                              << event.mouseButton.x << ", "
                              << event.mouseButton.y << ")\n";
                }
                // Check for other buttons if required (Middle, XButton1, XButton2)
            }
        }

        // Clear the window with white color
        window.clear(sf::Color::White);

        drawBoard(window);
        drawPieces(window);

        // End the current frame
        window.display();
    }

    return 0;
}