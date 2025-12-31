#pragma once
#include <SFML/Graphics.hpp>
#include "render.h"
#include "state.h"
#include "database.h"
#include "types.h"
#include "chessboard.h"
#include <vector>

class Analysis
{
public:
    void drawAnalysisScreen(sf::RenderWindow &window, Render &render);
    void handleEvent(sf::Event &event, sf::RenderWindow &window);

private:
    enum class AnalysisMode
    {
        List,      // Showing game list
        Board      // Showing board analysis
    };
    
    AnalysisMode currentMode = AnalysisMode::List;
    bool gameSelected = false;
    int selectedGameId = -1;
    float scrollOffset = 0.0f;
    float currentListHeight = 500.0f; // Current height of the list container
    std::vector<Types::GameRecord> games;
    bool gamesLoaded = false;
    Types::GameRecord selectedGame; // Store the loaded game data
    static constexpr float ITEM_HEIGHT = 60.0f;
    static constexpr float SCROLL_SPEED = 20.0f;
    
    // Double-click detection
    int lastClickedIndex = -1;
    sf::Clock doubleClickClock;
    static constexpr float DOUBLE_CLICK_TIME = 0.5f; // 500ms for double-click
    
    // Analysis mode state
    int currentMoveIndex = 0; // 0 = initial position, 1 = after first move, etc.
    Types::Board initialBoard; // Store the initial board state
    
    void loadGames();
    void handleScrolling(sf::Event &event, float listHeight);
    void drawGameList(sf::RenderWindow &window, const sf::RectangleShape &container);
    std::string formatGameInfo(const Types::GameRecord &game);
    
    // Analysis mode functions
    void initializeAnalysisMode();
    void applyMoveToBoard(const Types::Turn &turn);
    void undoMoveFromBoard(const Types::Turn &turn);
    void setBoardToMove(int moveIndex);
    void drawAnalysisBoard(sf::RenderWindow &window, Render &render);
    void drawNavigationControls(sf::RenderWindow &window);
    void drawMoveList(sf::RenderWindow &window, const sf::RectangleShape &panel);
    void drawCapturedPiecesPanel(sf::RenderWindow &window, const sf::RectangleShape &panel);
    std::string formatMove(const Types::Turn &turn, int moveNumber);
};