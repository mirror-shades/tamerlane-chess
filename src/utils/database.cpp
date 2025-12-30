// Copyright 2024. mirror-shades. GPL-2.0 License.
#include "database.h"
#include "types.h"
#include "state.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <chrono>

std::string Database::getGamesDirectory() {
    return "games/";
}

std::string Database::generateGameFilename(int id) {
    // Create games directory if it doesn't exist
    std::filesystem::create_directories(getGamesDirectory());
    
    // Format: games/game_001.csv, games/game_042.csv, etc.
    std::stringstream ss;
    ss << getGamesDirectory() << "game_" << std::setfill('0') << std::setw(3) << id << ".csv";
    return ss.str();
}

bool Database::saveGame(const Types::GameRecord& game) {
    std::string filename = generateGameFilename(game.id);
    return writeGameToCSV(game, filename);
}

bool Database::writeGameToCSV(const Types::GameRecord& game, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return false;
    }

    // Write header/metadata section
    file << "# Game Metadata" << std::endl;
    file << "id," << game.id << std::endl;
    file << "timestamp," << game.timestamp << std::endl;
    file << "whitePlayer," << game.whitePlayer << std::endl;
    file << "blackPlayer," << game.blackPlayer << std::endl;
    file << "result," << game.result << std::endl;
    file << "totalMoves," << game.totalMoves << std::endl;
    file << "duration," << game.duration << std::endl;
    file << std::endl; // Empty line separator
    
    // Write move history header
    file << "# Move History" << std::endl;
    file << "turn,player,fromX,fromY,toX,toY,pieceMoved,pieceCaptured,score" << std::endl;
    
    // Write each move
    for (const auto& turn : game.turnHistory) {
        file << turn.turn << ","
             << turn.player << ","
             << turn.initialSquare.x << ","
             << turn.initialSquare.y << ","
             << turn.finalSquare.x << ","
             << turn.finalSquare.y << ","
             << turn.pieceMoved.toString() << ","
             << turn.pieceCaptured.toString() << ","
             << turn.score << std::endl;
    }
    
    file.close();
    return true;
}

Types::GameRecord Database::parseGameFromCSV(const std::string& filepath) {
    Types::GameRecord game;  // Default constructor initializes id to -1
    
    std::ifstream file(filepath);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filepath << std::endl;
        return game;
    }
    
    bool readingMoves = false;
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check if we've reached the move history header
        if (line.find("turn,player,fromX") == 0) {
            readingMoves = true;
            continue;
        }
        
        if (!readingMoves) {
            // Parse metadata
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                std::string key = line.substr(0, commaPos);
                std::string value = line.substr(commaPos + 1);
                
                if (key == "id") game.id = std::stoi(value);
                else if (key == "timestamp") game.timestamp = value;
                else if (key == "whitePlayer") game.whitePlayer = value;
                else if (key == "blackPlayer") game.blackPlayer = value;
                else if (key == "result") game.result = value;
                else if (key == "totalMoves") game.totalMoves = std::stoi(value);
                else if (key == "duration") game.duration = std::stof(value);
            }
        } else {
            // Parse move data
            std::stringstream ss(line);
            std::string token;
            Types::Turn turn;
            
            if (std::getline(ss, token, ',')) turn.turn = std::stoi(token);
            if (std::getline(ss, token, ',')) turn.player = token[0];
            if (std::getline(ss, token, ',')) turn.initialSquare.x = std::stoi(token);
            if (std::getline(ss, token, ',')) turn.initialSquare.y = std::stoi(token);
            if (std::getline(ss, token, ',')) turn.finalSquare.x = std::stoi(token);
            if (std::getline(ss, token, ',')) turn.finalSquare.y = std::stoi(token);
            if (std::getline(ss, token, ',')) turn.pieceMoved = Types::Piece(token);
            if (std::getline(ss, token, ',')) turn.pieceCaptured = Types::Piece(token);
            if (std::getline(ss, token, ',')) turn.score = std::stof(token);
            
            game.turnHistory.push_back(turn);
        }
    }
    
    file.close();
    return game;
}

Types::GameRecord Database::loadGame(int gameId) {
    std::string filename = generateGameFilename(gameId);
    return parseGameFromCSV(filename);
}

std::vector<Types::GameRecord> Database::loadGameList() {
    std::vector<Types::GameRecord> games;
    std::string gamesDir = getGamesDirectory();
    
    // Check if games directory exists
    if (!std::filesystem::exists(gamesDir)) {
        return games; // Return empty list if no games directory
    }
    
    // Iterate through all CSV files in the games directory
    for (const auto& entry : std::filesystem::directory_iterator(gamesDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            Types::GameRecord game = parseGameFromCSV(entry.path().string());
            if (game.id >= 0) { // Valid game
                games.push_back(game);
            }
        }
    }
    
    // Sort by ID (most recent first, assuming higher ID = newer game)
    std::sort(games.begin(), games.end(), [](const Types::GameRecord& a, const Types::GameRecord& b) {
        return a.id > b.id;
    });
    
    return games;
}

bool Database::deleteGame(int gameId) {
    std::string filename = generateGameFilename(gameId);
    
    try {
        if (std::filesystem::exists(filename)) {
            return std::filesystem::remove(filename);
        }
        return false; // File doesn't exist
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting game file: " << e.what() << std::endl;
        return false;
    }
}

std::string Database::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

std::string Database::getActiveGameFilename() {
    std::filesystem::create_directories(getGamesDirectory());
    return getGamesDirectory() + "active_game.csv";
}

int Database::getNextGameId() {
    // Find the highest existing game ID and return next one
    std::vector<Types::GameRecord> games = loadGameList();
    int maxId = 0;
    for (const auto& game : games) {
        if (game.id > maxId) {
            maxId = game.id;
        }
    }
    return maxId + 1;
}

bool Database::saveActiveGame() {
    // Only save if we have a valid game ID and some moves
    if (State::currentGameId < 0 || State::turnHistory.empty()) {
        return false;
    }
    
    Types::GameRecord game;
    game.id = State::currentGameId;
    game.timestamp = getCurrentTimestamp();
    
    // Determine player types based on State
    if (State::aiVsAiMode) {
        game.whitePlayer = "AI";
        game.blackPlayer = "AI";
    } else if (State::aiActive) {
        // Human vs AI - determine which is which based on who starts
        // White always starts, so if AI is active, check if it's white's turn
        if (State::turns % 2 == 1) {
            // White's turn, so white is human
            game.whitePlayer = "Human";
            game.blackPlayer = "AI";
        } else {
            // Black's turn, so black is AI
            game.whitePlayer = "Human";
            game.blackPlayer = "AI";
        }
    } else {
        game.whitePlayer = "Human";
        game.blackPlayer = "Human";
    }
    
    game.result = "In Progress";
    game.totalMoves = State::turnHistory.size();
    game.duration = State::gameStartClock.getElapsedTime().asSeconds();
    game.turnHistory = State::turnHistory;
    
    std::string filename = getActiveGameFilename();
    return writeGameToCSV(game, filename);
}

bool Database::saveCompletedGame() {
    // Only save if we have a valid game ID
    if (State::currentGameId < 0) {
        return false;
    }
    
    Types::GameRecord game;
    game.id = State::currentGameId;
    game.timestamp = getCurrentTimestamp();
    
    // Determine player types based on State
    if (State::aiVsAiMode) {
        game.whitePlayer = "AI";
        game.blackPlayer = "AI";
    } else if (State::aiActive) {
        game.whitePlayer = "Human";
        game.blackPlayer = "AI";
    } else {
        game.whitePlayer = "Human";
        game.blackPlayer = "Human";
    }
    
    // Determine result
    if (State::winner == 'w') {
        game.result = "1-0";  // White wins
    } else if (State::winner == 'b') {
        game.result = "0-1";  // Black wins
    } else if (State::winner == 's' || State::winner == 'd') {
        game.result = "1/2-1/2";  // Draw
    } else {
        game.result = "Unknown";
    }
    
    game.totalMoves = State::turnHistory.size();
    game.duration = State::gameStartClock.getElapsedTime().asSeconds();
    game.turnHistory = State::turnHistory;
    
    // Save to regular game file
    bool success = saveGame(game);
    
    // Delete active game file if it exists
    std::string activeFile = getActiveGameFilename();
    if (std::filesystem::exists(activeFile)) {
        std::filesystem::remove(activeFile);
    }
    
    return success;
}

Types::GameRecord Database::loadActiveGame() {
    std::string filename = getActiveGameFilename();
    if (!std::filesystem::exists(filename)) {
        Types::GameRecord empty;  // Default constructor initializes id to -1
        return empty;
    }
    return parseGameFromCSV(filename);
}
