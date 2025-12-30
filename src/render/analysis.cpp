#include "analysis.h"
#include "render.h"
#include "state.h"
#include "utility.h"
#include <SFML/Graphics.hpp>

bool gameSelected = false;

void Analysis::drawAnalysisScreen(sf::RenderWindow &window, Render &render)
{
    if (gameSelected)
    {

    }
    else
    {

        render.tintScreen(window);
        sf::RectangleShape analysisMenu(sf::Vector2f(100, 100));
        analysisMenu.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50);
        analysisMenu.setFillColor(sf::Color::White);

        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
            sf::Color::White);

        Utility::drawButton(window,
                            backButton,
                            "Back",
                            20);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (Utility::isButtonClicked(backButton, mousePosition))
            {
                State::state = State::GameState::Menu;
            }
        }
    }   
}

