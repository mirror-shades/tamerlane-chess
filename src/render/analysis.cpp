#include "analysis.h"
#include "render.h"
#include "state.h"
#include "utility.h"
#include "database.h"
#include "globals.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

void Analysis::loadGames()
{
    if (!gamesLoaded)
    {
        std::vector<Types::GameRecord> allGames = Database::loadGameList();
        
        // Filter out active/in-progress games - only show completed games
        games.clear();
        for (const auto& game : allGames)
        {
            // Exclude games that are still in progress
            if (game.result != "In Progress" && game.result != "Unknown")
            {
                games.push_back(game);
            }
        }
        
        gamesLoaded = true;
        scrollOffset = 0.0f;
    }
}

void Analysis::handleEvent(sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        handleScrolling(event, currentListHeight);
    }
    
    // Handle mouse clicks
    if (event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
        
        // Handle back button clicks
        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
            sf::Color::White);
        
        if (Utility::isButtonClicked(backButton, mousePosition))
        {
            if (currentMode == AnalysisMode::Board)
            {
                // Back to list
                currentMode = AnalysisMode::List;
                gameSelected = false;
                selectedGameId = -1;
                currentMoveIndex = 0;
            }
            else if (gameSelected && currentMode == AnalysisMode::List)
            {
                // Back to list (from detail view)
                gameSelected = false;
                selectedGameId = -1;
            }
            else
            {
                // Back to menu
                State::state = State::GameState::Menu;
                gamesLoaded = false; // Reset so games reload next time
                gameSelected = false;
                selectedGameId = -1;
            }
            return;
        }
        
        // Handle navigation buttons in analysis mode
        if (currentMode == AnalysisMode::Board)
        {
            // Previous move button
            sf::RectangleShape prevButton = Utility::createButton(
                sf::Vector2f(120, 40),
                sf::Vector2f(window.getSize().x / 2 - 200, window.getSize().y - 80),
                sf::Color::White);
            
            if (Utility::isButtonClicked(prevButton, mousePosition))
            {
                if (currentMoveIndex > 0)
                {
                    currentMoveIndex--;
                    setBoardToMove(currentMoveIndex);
                }
                return;
            }
            
            // Next move button
            sf::RectangleShape nextButton = Utility::createButton(
                sf::Vector2f(120, 40),
                sf::Vector2f(window.getSize().x / 2 + 80, window.getSize().y - 80),
                sf::Color::White);
            
            if (Utility::isButtonClicked(nextButton, mousePosition))
            {
                if (currentMoveIndex < static_cast<int>(selectedGame.turnHistory.size()))
                {
                    currentMoveIndex++;
                    setBoardToMove(currentMoveIndex);
                }
                return;
            }
            
            // First move button
            sf::RectangleShape firstButton = Utility::createButton(
                sf::Vector2f(120, 40),
                sf::Vector2f(window.getSize().x / 2 - 320, window.getSize().y - 80),
                sf::Color::White);
            
            if (Utility::isButtonClicked(firstButton, mousePosition))
            {
                currentMoveIndex = 0;
                setBoardToMove(0);
                return;
            }
            
            // Last move button
            sf::RectangleShape lastButton = Utility::createButton(
                sf::Vector2f(120, 40),
                sf::Vector2f(window.getSize().x / 2 + 200, window.getSize().y - 80),
                sf::Color::White);
            
            if (Utility::isButtonClicked(lastButton, mousePosition))
            {
                currentMoveIndex = static_cast<int>(selectedGame.turnHistory.size());
                setBoardToMove(currentMoveIndex);
                return;
            }
        }
        
        // Handle keyboard navigation in analysis mode
        if (currentMode == AnalysisMode::Board && event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
            {
                // Previous move
                if (currentMoveIndex > 0)
                {
                    currentMoveIndex--;
                    setBoardToMove(currentMoveIndex);
                }
            }
            else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
            {
                // Next move
                if (currentMoveIndex < static_cast<int>(selectedGame.turnHistory.size()))
                {
                    currentMoveIndex++;
                    setBoardToMove(currentMoveIndex);
                }
            }
            else if (event.key.code == sf::Keyboard::Home)
            {
                // First move
                currentMoveIndex = 0;
                setBoardToMove(0);
            }
            else if (event.key.code == sf::Keyboard::End)
            {
                // Last move
                currentMoveIndex = static_cast<int>(selectedGame.turnHistory.size());
                setBoardToMove(currentMoveIndex);
            }
        }
        
        // Handle game list clicks (only when not in detail view or analysis mode)
        if (!gameSelected && currentMode == AnalysisMode::List)
        {
            // Calculate list bounds
            float listWidth = 700.0f - 20.0f;
            float listHeight = 550.0f - 50.0f;
            sf::Vector2f listPos(
                window.getSize().x / 2.0f - 350.0f + 10.0f,
                window.getSize().y / 2.0f - 400.0f + 50.0f
            );
            
            // Check if click is within the list area
            if (mousePosition.x >= listPos.x &&
                mousePosition.x <= listPos.x + listWidth &&
                mousePosition.y >= listPos.y &&
                mousePosition.y <= listPos.y + listHeight)
            {
                // Calculate which game was clicked
                float relativeY = mousePosition.y - listPos.y + scrollOffset;
                int clickedIndex = static_cast<int>(relativeY / ITEM_HEIGHT);
                
                if (clickedIndex >= 0 && clickedIndex < static_cast<int>(games.size()))
                {
                    // Check for double-click
                    float timeSinceLastClick = doubleClickClock.getElapsedTime().asSeconds();
                    if (lastClickedIndex == clickedIndex && timeSinceLastClick < DOUBLE_CLICK_TIME)
                    {
                        // Double-click detected - launch analysis mode
                        selectedGameId = games[clickedIndex].id;
                        selectedGame = Database::loadGame(selectedGameId);
                        gameSelected = true;
                        currentMode = AnalysisMode::Board;
                        initializeAnalysisMode();
                        lastClickedIndex = -1; // Reset
                    }
                    else
                    {
                        // Single click - just highlight
                        selectedGameId = games[clickedIndex].id;
                        lastClickedIndex = clickedIndex;
                        doubleClickClock.restart();
                    }
                }
            }
        }
    }
}

void Analysis::handleScrolling(sf::Event &event, float listHeight)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        float contentHeight = static_cast<float>(games.size()) * ITEM_HEIGHT;
        float maxScroll = std::max(0.0f, contentHeight - listHeight);
        
        // In SFML, positive delta means scrolling down (toward user)
        // To show content further down, we need to increase scrollOffset
        // Negate delta to fix reversed scrolling direction
        scrollOffset -= event.mouseWheelScroll.delta * SCROLL_SPEED;
        scrollOffset = std::max(0.0f, std::min(scrollOffset, maxScroll));
    }
}

std::string Analysis::formatGameInfo(const Types::GameRecord &game)
{
    std::stringstream ss;
    ss << "Game #" << std::setfill('0') << std::setw(3) << game.id;
    ss << " | " << game.result;
    ss << " | " << game.whitePlayer << " vs " << game.blackPlayer;
    ss << " | " << game.totalMoves << " moves";
    
    // Format duration
    int minutes = static_cast<int>(game.duration) / 60;
    int seconds = static_cast<int>(game.duration) % 60;
    ss << " | " << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    
    return ss.str();
}

void Analysis::drawGameList(sf::RenderWindow &window, const sf::RectangleShape &container)
{
    if (games.empty())
    {
        // Draw "No games" message
        sf::Text noGamesText;
        noGamesText.setFont(Utility::getFont());
        noGamesText.setString("No games found");
        noGamesText.setCharacterSize(24);
        noGamesText.setFillColor(sf::Color::White);
        noGamesText.setPosition(
            container.getPosition().x + container.getSize().x / 2 - 90,
            container.getPosition().y + container.getSize().y / 2 - 50
        );
        window.draw(noGamesText);
        return;
    }

    // Create a view for clipping
    sf::View listView;
    sf::FloatRect viewport(
        container.getPosition().x / static_cast<float>(window.getSize().x),
        container.getPosition().y / static_cast<float>(window.getSize().y),
        container.getSize().x / static_cast<float>(window.getSize().x),
        container.getSize().y / static_cast<float>(window.getSize().y)
    );
    listView.setViewport(viewport);
    listView.setSize(container.getSize());
    // Center the view on the content, accounting for scroll offset
    // Content starts at y=10, so to show it at the top, center should be at 10 + viewHeight/2
    float baseCenterY = 10.0f + container.getSize().y / 2.0f;
    listView.setCenter(container.getSize().x / 2.0f, baseCenterY + scrollOffset);

    // Save the current view
    sf::View oldView = window.getView();
    window.setView(listView);

    // Calculate visible bounds in view coordinates
    float viewTop = baseCenterY + scrollOffset - container.getSize().y / 2.0f;
    float viewBottom = baseCenterY + scrollOffset + container.getSize().y / 2.0f;

    // Draw game items
    float yPos = 10.0f;
    for (size_t i = 0; i < games.size(); ++i)
    {
        const auto &game = games[i];
        
        // Only draw if within visible area (check if item overlaps with view bounds)
        if (yPos + ITEM_HEIGHT >= viewTop && yPos <= viewBottom)
        {
            // Draw item background (highlight if selected)
            sf::RectangleShape itemBackground(sf::Vector2f(container.getSize().x - 20, ITEM_HEIGHT - 5));
            itemBackground.setPosition(10, yPos);
            
            if (game.id == selectedGameId)
            {
                itemBackground.setFillColor(sf::Color(100, 100, 150));
            }
            else
            {
                itemBackground.setFillColor(sf::Color(50, 50, 50));
            }
            window.draw(itemBackground);

            // Draw game info text
            sf::Text gameText;
            gameText.setFont(Utility::getFont());
            gameText.setString(formatGameInfo(game));
            gameText.setCharacterSize(18);
            gameText.setFillColor(sf::Color::White);
            gameText.setPosition(15, yPos + 15);
            window.draw(gameText);

            // Draw timestamp on second line
            sf::Text timestampText;
            timestampText.setFont(Utility::getFont());
            timestampText.setString(game.timestamp);
            timestampText.setCharacterSize(14);
            timestampText.setFillColor(sf::Color(200, 200, 200));
            timestampText.setPosition(15, yPos + 38);
            window.draw(timestampText);
        }

        yPos += ITEM_HEIGHT;
    }

    // Restore the original view
    window.setView(oldView);
}

void Analysis::initializeAnalysisMode()
{
    // Reset board to initial state (masculine board)
    chessboard.resetBoard();
    initialBoard = chessboard.getBoardState();
    currentMoveIndex = 0;
    
    // Apply moves up to turn 1 (first move)
    if (!selectedGame.turnHistory.empty() && selectedGame.turnHistory[0].turn == 1)
    {
        currentMoveIndex = 1;
        setBoardToMove(1);
    }
}

void Analysis::applyMoveToBoard(const Types::Turn &turn)
{
    // Move the piece
    chessboard.setCell(turn.initialSquare, "---");
    chessboard.setCell(turn.finalSquare, turn.pieceMoved);
}

void Analysis::undoMoveFromBoard(const Types::Turn &turn)
{
    // Restore the captured piece (or empty square)
    chessboard.setCell(turn.finalSquare, turn.pieceCaptured);
    // Restore the moved piece to its original position
    chessboard.setCell(turn.initialSquare, turn.pieceMoved);
}

void Analysis::setBoardToMove(int moveIndex)
{
    // Reset to initial board
    chessboard.setBoard(initialBoard);
    
    // Clear and rebuild State::turnHistory to match current position
    State::turnHistory.clear();
    
    // Apply all moves up to moveIndex
    for (int i = 0; i < moveIndex && i < static_cast<int>(selectedGame.turnHistory.size()); ++i)
    {
        applyMoveToBoard(selectedGame.turnHistory[i]);
        // Add to State::turnHistory so highlightPreviousMove works
        State::turnHistory.push_back(selectedGame.turnHistory[i]);
    }
}

void Analysis::drawNavigationControls(sf::RenderWindow &window)
{
    // Draw move counter
    sf::Text moveCounter;
    moveCounter.setFont(Utility::getFont());
    moveCounter.setString("Move: " + std::to_string(currentMoveIndex) + " / " + 
                         std::to_string(selectedGame.turnHistory.size()));
    moveCounter.setCharacterSize(20);
    moveCounter.setFillColor(sf::Color::White);
    moveCounter.setPosition(window.getSize().x / 2 - 80, window.getSize().y - 120);
    window.draw(moveCounter);
    
    // Draw navigation buttons
    sf::RectangleShape firstButton = Utility::createButton(
        sf::Vector2f(120, 40),
        sf::Vector2f(window.getSize().x / 2 - 320, window.getSize().y - 80),
        sf::Color::White);
    Utility::drawButton(window, firstButton, "First", 16);
    
    sf::RectangleShape prevButton = Utility::createButton(
        sf::Vector2f(120, 40),
        sf::Vector2f(window.getSize().x / 2 - 200, window.getSize().y - 80),
        sf::Color::White);
    Utility::drawButton(window, prevButton, "Previous", 16);
    
    sf::RectangleShape nextButton = Utility::createButton(
        sf::Vector2f(120, 40),
        sf::Vector2f(window.getSize().x / 2 + 80, window.getSize().y - 80),
        sf::Color::White);
    Utility::drawButton(window, nextButton, "Next", 16);
    
    sf::RectangleShape lastButton = Utility::createButton(
        sf::Vector2f(120, 40),
        sf::Vector2f(window.getSize().x / 2 + 200, window.getSize().y - 80),
        sf::Color::White);
    Utility::drawButton(window, lastButton, "Last", 16);
    
    // Draw back button
    sf::RectangleShape backButton = Utility::createButton(
        sf::Vector2f(200, 50),
        sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
        sf::Color::White);
    Utility::drawButton(window, backButton, "Back to List", 20);
    
    // Draw game info
    sf::Text gameInfo;
    gameInfo.setFont(Utility::getFont());
    gameInfo.setString("Game #" + std::to_string(selectedGame.id) + " - " + selectedGame.result);
    gameInfo.setCharacterSize(24);
    gameInfo.setFillColor(sf::Color::White);
    gameInfo.setPosition(window.getSize().x / 2 - 150, 10);
    window.draw(gameInfo);
}

std::string Analysis::formatMove(const Types::Turn &turn, int moveNumber)
{
    std::stringstream ss;
    ss << moveNumber << ". ";
    // Format move (e.g., "e2-e4")
    ss << "(" << turn.initialSquare.x << "," << turn.initialSquare.y << ")>"
       << "(" << turn.finalSquare.x << "," << turn.finalSquare.y << ")";
    
    if (turn.pieceCaptured.toString() != "---")
    {
        ss << " x " << turn.pieceCaptured.toString();
    }
    
    return ss.str();
}

void Analysis::drawMoveList(sf::RenderWindow &window, const sf::RectangleShape &panel)
{
    // Draw title
    sf::Text title;
    title.setFont(Utility::getFont());
    title.setString("Move List");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setPosition(panel.getPosition().x + 10, panel.getPosition().y + 10);
    window.draw(title);
    
    // Draw moves
    float yPos = panel.getPosition().y + 40;
    float maxY = panel.getPosition().y + panel.getSize().y - 10;
    
    for (size_t i = 0; i < selectedGame.turnHistory.size() && yPos < maxY; ++i)
    {
        const auto &turn = selectedGame.turnHistory[i];
        
        // Highlight current move
        if (static_cast<int>(i) == currentMoveIndex - 1)
        {
            sf::RectangleShape highlight(sf::Vector2f(panel.getSize().x - 20, 20));
            highlight.setPosition(panel.getPosition().x + 10, yPos - 2);
            highlight.setFillColor(sf::Color(100, 100, 150, 180));
            window.draw(highlight);
        }
        
        sf::Text moveText;
        moveText.setFont(Utility::getFont());
        moveText.setString(formatMove(turn, turn.turn));
        moveText.setCharacterSize(14);
        moveText.setFillColor(
            static_cast<int>(i) == currentMoveIndex - 1 ? sf::Color::Yellow : sf::Color::White
        );
        moveText.setPosition(panel.getPosition().x + 15, yPos);
        window.draw(moveText);
        
        yPos += 22;
    }
}

void Analysis::drawCapturedPiecesPanel(sf::RenderWindow &window, const sf::RectangleShape &panel)
{
    float yPos = panel.getPosition().y + 10;
    
    // Draw game info
    sf::Text title;
    title.setFont(Utility::getFont());
    title.setString("Game Info");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setPosition(panel.getPosition().x + 10, yPos);
    window.draw(title);
    yPos += 35;
    
    auto drawInfo = [&](const std::string &label, const std::string &value) {
        sf::Text labelText;
        labelText.setFont(Utility::getFont());
        labelText.setString(label + ":");
        labelText.setCharacterSize(14);
        labelText.setFillColor(sf::Color(200, 200, 200));
        labelText.setPosition(panel.getPosition().x + 10, yPos);
        window.draw(labelText);
        
        sf::Text valueText;
        valueText.setFont(Utility::getFont());
        valueText.setString(value);
        valueText.setCharacterSize(14);
        valueText.setFillColor(sf::Color::White);
        valueText.setPosition(panel.getPosition().x + 120, yPos);
        window.draw(valueText);
        
        yPos += 20;
    };
    
    drawInfo("Game #", std::to_string(selectedGame.id));
    drawInfo("Result", selectedGame.result);
    drawInfo("White", selectedGame.whitePlayer);
    drawInfo("Black", selectedGame.blackPlayer);
    drawInfo("Moves", std::to_string(currentMoveIndex) + " / " + std::to_string(selectedGame.turnHistory.size()));
    
    yPos += 20;
    
    // Draw captured pieces
    sf::Text capturedTitle;
    capturedTitle.setFont(Utility::getFont());
    capturedTitle.setString("Captured Pieces");
    capturedTitle.setCharacterSize(18);
    capturedTitle.setFillColor(sf::Color::White);
    capturedTitle.setPosition(panel.getPosition().x + 10, yPos);
    window.draw(capturedTitle);
    yPos += 30;
    
    // Calculate captured pieces up to current move
    std::vector<std::string> whiteCaptured, blackCaptured;
    for (int i = 0; i < currentMoveIndex && i < static_cast<int>(selectedGame.turnHistory.size()); ++i)
    {
        const auto &turn = selectedGame.turnHistory[i];
        if (turn.pieceCaptured.toString() != "---")
        {
            if (turn.player == 'w')
                blackCaptured.push_back(turn.pieceCaptured.toString());
            else
                whiteCaptured.push_back(turn.pieceCaptured.toString());
        }
    }
    
    // Draw white captured pieces
    sf::Text whiteLabel;
    whiteLabel.setFont(Utility::getFont());
    whiteLabel.setString("White captured:");
    whiteLabel.setCharacterSize(14);
    whiteLabel.setFillColor(sf::Color(200, 200, 200));
    whiteLabel.setPosition(panel.getPosition().x + 10, yPos);
    window.draw(whiteLabel);
    yPos += 20;
    
    if (blackCaptured.empty())
    {
        sf::Text none;
        none.setFont(Utility::getFont());
        none.setString("None");
        none.setCharacterSize(12);
        none.setFillColor(sf::Color(150, 150, 150));
        none.setPosition(panel.getPosition().x + 10, yPos);
        window.draw(none);
        yPos += 20;
    }
    else
    {
        std::string capturedStr;
        for (const auto &piece : blackCaptured)
        {
            capturedStr += piece + " ";
        }
        sf::Text capturedText;
        capturedText.setFont(Utility::getFont());
        capturedText.setString(capturedStr);
        capturedText.setCharacterSize(12);
        capturedText.setFillColor(sf::Color::White);
        capturedText.setPosition(panel.getPosition().x + 10, yPos);
        window.draw(capturedText);
        yPos += 20;
    }
    
    // Draw black captured pieces
    sf::Text blackLabel;
    blackLabel.setFont(Utility::getFont());
    blackLabel.setString("Black captured:");
    blackLabel.setCharacterSize(14);
    blackLabel.setFillColor(sf::Color(200, 200, 200));
    blackLabel.setPosition(panel.getPosition().x + 10, yPos);
    window.draw(blackLabel);
    yPos += 20;
    
    if (whiteCaptured.empty())
    {
        sf::Text none;
        none.setFont(Utility::getFont());
        none.setString("None");
        none.setCharacterSize(12);
        none.setFillColor(sf::Color(150, 150, 150));
        none.setPosition(panel.getPosition().x + 10, yPos);
        window.draw(none);
    }
    else
    {
        std::string capturedStr;
        for (const auto &piece : whiteCaptured)
        {
            capturedStr += piece + " ";
        }
        sf::Text capturedText;
        capturedText.setFont(Utility::getFont());
        capturedText.setString(capturedStr);
        capturedText.setCharacterSize(12);
        capturedText.setFillColor(sf::Color::White);
        capturedText.setPosition(panel.getPosition().x + 10, yPos);
        window.draw(capturedText);
    }
}

void Analysis::drawAnalysisBoard(sf::RenderWindow &window, [[maybe_unused]] Render &render)
{
    // Note: Board and pieces are drawn in game.cpp with zoomed view
    // Here we only draw UI panels that are in window coordinates
    
    // Draw UI panels on the sides (in window coordinates, not affected by zoom)
    // Left panel: Move list
    float panelWidth = 160.0f;
    float panelHeight = static_cast<float>(window.getSize().y) - 400.0f;
    
    sf::RectangleShape moveListPanel(sf::Vector2f(panelWidth, panelHeight));
    moveListPanel.setPosition(9, 235);
    moveListPanel.setFillColor(sf::Color(30, 30, 30, 240));
    moveListPanel.setOutlineColor(sf::Color::White);
    moveListPanel.setOutlineThickness(2);
    window.draw(moveListPanel);
    
    // Right panel: Captured pieces and game info
    sf::RectangleShape infoPanel(sf::Vector2f(panelWidth, panelHeight));
    infoPanel.setPosition(static_cast<float>(window.getSize().x) - panelWidth - 10, 50);
    infoPanel.setFillColor(sf::Color(30, 30, 30, 240));
    infoPanel.setOutlineColor(sf::Color::White);
    infoPanel.setOutlineThickness(2);
    window.draw(infoPanel);
    
    // Draw move list
    drawMoveList(window, moveListPanel);
    
    // Draw captured pieces and game info
    drawCapturedPiecesPanel(window, infoPanel);
    
    // Draw navigation controls at bottom
    drawNavigationControls(window);
}

void Analysis::drawAnalysisScreen(sf::RenderWindow &window, Render &render)
{
    if (currentMode == AnalysisMode::Board)
    {
        // Analysis board mode - show the chessboard
        drawAnalysisBoard(window, render);
    }
    else if (gameSelected)
    {
        // Game detail view
        render.tintScreen(window);
        
        sf::RectangleShape gameDetailPanel(sf::Vector2f(800, 600));
        gameDetailPanel.setPosition(window.getSize().x / 2 - 400, window.getSize().y / 2 - 300);
        gameDetailPanel.setFillColor(sf::Color(30, 30, 30));
        gameDetailPanel.setOutlineColor(sf::Color::White);
        gameDetailPanel.setOutlineThickness(2);
        window.draw(gameDetailPanel);
        
        // Draw title
        sf::Text titleText;
        titleText.setFont(Utility::getFont());
        titleText.setString("Game #" + std::to_string(selectedGame.id) + " - " + selectedGame.result);
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(
            gameDetailPanel.getPosition().x + gameDetailPanel.getSize().x / 2 - 150,
            gameDetailPanel.getPosition().y + 10
        );
        window.draw(titleText);
        
        // Draw game info
        float yPos = gameDetailPanel.getPosition().y + 60;
        float xPos = gameDetailPanel.getPosition().x + 20;
        
        auto drawInfoLine = [&](const std::string& label, const std::string& value, float y) {
            sf::Text labelText;
            labelText.setFont(Utility::getFont());
            labelText.setString(label + ":");
            labelText.setCharacterSize(18);
            labelText.setFillColor(sf::Color(200, 200, 200));
            labelText.setPosition(xPos, y);
            window.draw(labelText);
            
            sf::Text valueText;
            valueText.setFont(Utility::getFont());
            valueText.setString(value);
            valueText.setCharacterSize(18);
            valueText.setFillColor(sf::Color::White);
            valueText.setPosition(xPos + 150, y);
            window.draw(valueText);
        };
        
        drawInfoLine("White Player", selectedGame.whitePlayer, yPos);
        yPos += 35;
        drawInfoLine("Black Player", selectedGame.blackPlayer, yPos);
        yPos += 35;
        drawInfoLine("Result", selectedGame.result, yPos);
        yPos += 35;
        drawInfoLine("Total Moves", std::to_string(selectedGame.totalMoves), yPos);
        yPos += 35;
        
        // Format duration
        int minutes = static_cast<int>(selectedGame.duration) / 60;
        int seconds = static_cast<int>(selectedGame.duration) % 60;
        std::stringstream durationStr;
        durationStr << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
        drawInfoLine("Duration", durationStr.str(), yPos);
        yPos += 35;
        drawInfoLine("Date", selectedGame.timestamp, yPos);
        
        // Draw back button
        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
            sf::Color::White);
        
        Utility::drawButton(window, backButton, "Back to List", 20);
    }
    else
    {
        render.tintScreen(window);

        // Load games if not already loaded
        loadGames();

        sf::RectangleShape gameExplorer(sf::Vector2f(700, 550));
        gameExplorer.setPosition(window.getSize().x / 2 - 350, window.getSize().y / 2 - 400);
        gameExplorer.setFillColor(sf::Color(30, 30, 30));
        gameExplorer.setOutlineColor(sf::Color::White);
        gameExplorer.setOutlineThickness(2);
        window.draw(gameExplorer);

        // Draw title
        sf::Text titleText;
        titleText.setFont(Utility::getFont());
        titleText.setString("Game Explorer");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(
            gameExplorer.getPosition().x + gameExplorer.getSize().x / 2 - 90,
            gameExplorer.getPosition().y + 10
        );
        window.draw(titleText);

        // Create a container for the list (below the title)
        // Calculate dimensions dynamically from gameExplorer
        float listWidth = gameExplorer.getSize().x - 20.0f; // 10px margin on each side
        float listHeight = gameExplorer.getSize().y - 50.0f; // Leave space for title (40px) + margin (10px)
        currentListHeight = listHeight; // Store for use in handleEvent
        sf::RectangleShape listContainer(sf::Vector2f(listWidth, listHeight));
        listContainer.setPosition(gameExplorer.getPosition().x + 10, gameExplorer.getPosition().y + 50);
        listContainer.setFillColor(sf::Color::Transparent);
        
        // Draw game list with clipping
        drawGameList(window, listContainer);

        // Draw back button
        sf::RectangleShape backButton = Utility::createButton(
            sf::Vector2f(200, 50),
            sf::Vector2f((window.getSize().x) / 2 - 100, window.getSize().y / 2 + 200),
            sf::Color::White);

        Utility::drawButton(window,
                            backButton,
                            "Back",
                            20);

        // Handle back button clicks (moved to handleEvent for proper event handling)
    }   
}

