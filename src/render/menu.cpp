// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "chessboard.h"
#include "utility.h"
#include "state.h"
#include "menu.h"
#include "render.h"

extern Chessboard chessboard;
extern Render render;

sf::RectangleShape slider;
sf::CircleShape sliderHandle;

void Menu::drawSlider(sf::RenderWindow &window, sf::Font &font)
{
    const float sliderWidth = 200.0f;
    const float sliderHeight = 10.0f;
    const float sliderY = window.getSize().y / 2 + 25;
    const float sliderX = (window.getSize().x - sliderWidth) / 2;

    slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    slider.setFillColor(sf::Color(150, 150, 150));
    slider.setPosition(sliderX, sliderY);
    window.draw(slider);

    const float handleRadius = 10.0f;
    const float handleOffset = (State::aiDifficulty - 1) * (sliderWidth / 9);
    const float handleX = sliderX + handleOffset;
    const float handleY = sliderY - handleRadius + sliderHeight / 2;

    sliderHandle.setRadius(handleRadius);
    sliderHandle.setFillColor(sf::Color::White);
    sliderHandle.setOutlineThickness(2);
    sliderHandle.setOutlineColor(sf::Color::Black);
    sliderHandle.setPosition(handleX - handleRadius, handleY);
    window.draw(sliderHandle);

    const int textSize = 20;
    const float textY = sliderY - 40;

    sf::Text difficultyText;
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(textSize);
    difficultyText.setFillColor(sf::Color::White);
    difficultyText.setString("AI Difficulty: " + std::to_string(State::aiDifficulty));

    const float textX = (window.getSize().x - difficultyText.getLocalBounds().width) / 2;
    difficultyText.setPosition(textX, textY);
    window.draw(difficultyText);
}

void Menu::drawMenuScreen(sf::RenderWindow &window)
{
    if (State::state == State::GameState::Menu ||
        State::state == State::GameState::AIOptions)
    {
        render.tintScreen(window);
        sf::Texture titleTexture;

        if (titleTexture.loadFromFile(render.findAssetsPath("images/title.png")))
        {
            sf::Sprite titleSprite(titleTexture);

            titleTexture.setSmooth(false);

            float scale = window.getSize().x /
                          static_cast<float>(titleTexture.getSize().x);
            titleSprite.setScale(scale, scale);

            // Center the sprite horizontally and position
            // at 1/4 of window height
            // No need to calculate xPos, as we're scaling to fit width
            float xPos = 0;
            float yPos = (window.getSize().y - titleTexture.getSize().y * scale) / 4.0f;

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

        static bool isPlayAsWhiteHighlighted = true;
        static bool isPlayAsBlackHighlighted = false;
        static bool isMascHighlighted = true;
        static bool isFemHighlighted = false;
        static bool isThirdHighlighted = false;
        static bool wasMousePressed = false;

        sf::RectangleShape aiButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x - 500) / 2 - 75, window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape aiVsAiButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape pvpButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 125, window.getSize().y / 2 - 100),
            sf::Color::White);

        sf::RectangleShape masc = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x - 500) / 2 - 75, window.getSize().y / 2 - 25),
            isMascHighlighted ? State::colourSelected : sf::Color::White);

        sf::RectangleShape fem = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 - 25),
            isFemHighlighted ? State::colourSelected : sf::Color::White);

        sf::RectangleShape third = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 125, window.getSize().y / 2 - 25),
            isThirdHighlighted ? State::colourSelected : sf::Color::White);

        sf::RectangleShape blitz = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 50),
            State::alt ? State::colourSelected : sf::Color::White);
            
        sf::RectangleShape analysis = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
            sf::Color::White);

        sf::RectangleShape aiBlackButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 + 50, window.getSize().y / 2 - 100),
            isPlayAsBlackHighlighted ? State::colourSelected : sf::Color::White);

        sf::RectangleShape aiWhiteButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 250, window.getSize().y / 2 - 100),
            isPlayAsWhiteHighlighted ? State::colourSelected : sf::Color::White);

        sf::RectangleShape aiPlayButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 75),
            sf::Color::White);

        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 150),
            sf::Color::White);

        sf::Font font;
        if (!font.loadFromFile(render.findAssetsPath("fonts/arial.ttf")))
        {
            std::cerr << "Failed to load font" << std::endl;
        }

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
            Utility::drawButton(window,
                                analysis,
                                "Analysis",
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
            drawSlider(window, font);
        }
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (mousePressed && !wasMousePressed &&
            State::state == State::GameState::Menu)
        {
            if (Utility::isButtonClicked(pvpButton, mousePosition))
            {
                Utility::initializeNewGame();
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
                Utility::initializeNewGame();
                State::state = State::GameState::Game;
                State::aiVsAiMode = true;
                State::aiVsAiClock.restart();
            }
            else if (Utility::isButtonClicked(blitz, mousePosition))
            {
                State::alt = !State::alt;
            }
            else if (Utility::isButtonClicked(analysis, mousePosition))
            {
                State::state = State::GameState::Analysis;
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
                Utility::initializeNewGame();
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
            if (slider.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                float newDifficulty = (mousePosition.x - slider.getPosition().x) / slider.getSize().x * 9 + 1;
                State::aiDifficulty = std::max(1, std::min(10, static_cast<int>(newDifficulty)));
            }
        }

        wasMousePressed = mousePressed;
    }
}
