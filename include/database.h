#pragma once
#include <vector>
#include <string>
#include "types.h"

class Database {
public:
    static bool saveGame(const Types::GameRecord& game);
    static std::vector<Types::GameRecord> loadGameList();
    static Types::GameRecord loadGame(int gameId);
    static bool deleteGame(int gameId);
    
    // Save completed game (when game ends)
    static bool saveCompletedGame();
    
    // Save active game state (for resume functionality)
    static bool saveActiveGame();
    
    // Load active game state (on startup to resume)
    static Types::GameRecord loadActiveGame();
    
    // Generate next game ID
    static int getNextGameId();
    
private:
    static std::string getGamesDirectory();
    static std::string generateGameFilename(int id);
    static std::string getActiveGameFilename();
    static Types::GameRecord parseGameFromCSV(const std::string& filepath);
    static bool writeGameToCSV(const Types::GameRecord& game, const std::string& filepath);
    static std::string getCurrentTimestamp();
};