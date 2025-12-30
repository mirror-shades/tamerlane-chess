#pragma once
#include <SFML/Graphics.hpp>
#include "render.h"
#include "state.h"

class Analysis
{
public:
    void drawAnalysisScreen(sf::RenderWindow &window, Render &render);

private:
    bool gameSelected = false;
};