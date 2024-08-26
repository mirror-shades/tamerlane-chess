#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include <iostream>

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