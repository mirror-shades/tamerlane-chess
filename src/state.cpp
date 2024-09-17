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
// Captured pieces
std::vector<std::string> State::whitePiecesCaptured;
std::vector<std::string> State::blackPiecesCaptured;
std::vector<Types::Turn> State::turnHistory;
State::GameState State::state = State::GameState::Menu;
char State::player = 'w';

bool State::isPieceSelected = false;
std::vector<Types::Coord> State::moveList;
std::string State::selectedPiece;
Types::Coord State::selectedSquare = {-1, -1};