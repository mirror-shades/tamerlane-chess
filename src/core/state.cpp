// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "state.h"

int State::turns = 1;
int State::aiDifficulty = 2;
bool State::ended = false;
bool State::isBlackKingInCheck = false;
bool State::isWhiteKingInCheck = false;
bool State::drawPossible = false;
bool State::alt = false;
bool State::gameOver = false;
char State::winner = '-';
bool State::aiVsAiMode = false;
bool State::aiActive = false;
bool State::aiMoveQueued = false;
bool State::animationActive = false;
bool State::renderNeeded = true;
sf::Clock State::aiVsAiClock;
sf::Clock State::gameStartClock;
int State::currentGameId = -1;

// Calculate time since last frame
sf::Clock State::deltaClock;
float State::deltaTime = State::deltaClock.restart().asSeconds();

// Images
std::map<std::string, sf::Sprite> State::images;
sf::Sprite State::backgroundSprite;
sf::Texture State::backgroundTexture;

// Captured pieces
std::vector<std::string> State::whitePiecesCaptured;
std::vector<std::string> State::blackPiecesCaptured;
std::vector<Types::Turn> State::turnHistory;
std::vector<std::string> State::positionHistory;
State::GameState State::state = State::GameState::Menu;
char State::player = 'w';

bool State::isPieceSelected = false;
std::vector<Types::Coord> State::moveList;
Types::Coord State::selectedSquare = {-1, -1};

// this needs to be a char array to avoid global string issues
Types::Piece State::selectedPiece;

// Colors for the chess board and piece highlighting
sf::Color State::colour1 = sf::Color(0xE5E5E5ff);
sf::Color State::colour2 = sf::Color(0x26403Cff);
sf::Color State::colourSelected = sf::Color(0x6290c8ff);
sf::Color State::colourPrevMove = sf::Color(0x6290c855);
sf::Color State::colourMove = sf::Color(0xFBFF1255);

// Camera/zoom system
State::ZoomLevel State::currentZoomLevel = State::ZoomLevel::ZoomedOut;
State::ZoomLevel State::previousZoomLevel = State::ZoomLevel::ZoomedOut;
bool State::isZooming = false;
float State::zoomProgress = 0.0f;
