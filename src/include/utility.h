#ifndef UTILITY_H
#define UTILITY_H

#include "types.h"
#include <SFML/Graphics.hpp>
#include <string>

class Utility
{
public:
    static Types::Coord calculateSquare(int x, int y);
    static bool clickInBoard(const int x, const int y);

    // New button-related functions
    static sf::RectangleShape createButton(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &fillColor);
    static void drawButton(sf::RenderWindow &window, const sf::RectangleShape &button, const std::string &text, const sf::Font &font, unsigned int characterSize);
    static bool isButtonClicked(const sf::RectangleShape &button, const sf::Vector2i &mousePosition);
};

#endif // UTILITY_H