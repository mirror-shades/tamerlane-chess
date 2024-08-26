#include "include/gameLogic.h"
#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include "include/ai.h"
#include <vector>
#include <random>
#include <stdexcept>

// Static member to hold the random number generator
static std::mt19937 rng(std::random_device{}());

Types::Turn AI::getAIMove(const char player, int turn, bool alt)
{
    GameLogic gameLogic;
    std::vector<Types::Turn> allPossibleMoves;

    // Get all possible moves for the AI player
    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Coord currentSquare = {col, row};
            std::string piece = chessboard.getPiece(currentSquare);

            if (piece[0] == player)
            {
                std::vector<Types::Coord> possibleMoves = gameLogic.getMoves(currentSquare, piece, player, alt);
                std::vector<Types::Coord> legalMoves = gameLogic.filterLegalMoves(possibleMoves, currentSquare, piece, player, alt);

                for (const auto &move : legalMoves)
                {
                    Types::Turn turn = {
                        0, // turns will be set later
                        player,
                        currentSquare,
                        move,
                        piece,
                        chessboard.getPiece(move)};
                    allPossibleMoves.push_back(turn);
                }
            }
        }
    }

    // Choose a random move
    if (!allPossibleMoves.empty())
    {
        std::uniform_int_distribution<> dist(0, allPossibleMoves.size() - 1);
        int randomIndex = dist(rng);
        return allPossibleMoves[randomIndex];
    }

    // No legal moves available
    throw std::runtime_error("No legal moves available for AI player");
}
