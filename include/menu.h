// Copyright 2024. mirror-shades. GPL-2.0 License.
#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
private:
    sf::Shader logoShader;
    sf::Clock logoAnimationClock;
    bool shaderLoaded = false;
    
public:
    void drawMenuScreen(sf::RenderWindow &window);
    void drawSlider(sf::RenderWindow &window);
};
