#include "include/gameLogic.h"
#include "include/types.h"
#include "include/globals.h"
#include "include/utility.h"
#include "include/ai.h"
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <limits>

// Static member to hold the random number generator
static std::mt19937 rng(std::random_device{}());

// Piece values for the greedy algorithm
const std::map<char, float> pieceValues = {
    {'K', 3.5}, {'p', 1.0}, {'M', 3.0}, {'C', 2.0}, {'G', 4.0}, {'R', 5.0}, {'T', 2.5}, {'E', 1.5}, {'W', 2.0}, {'V', 1.5}, {'A', 1.5}};

Types::Turn AI::getRandomAIMove(const char player, int turn, bool alt)
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

Types::Turn AI::getGreedyAIMove(const char player, int turn, bool alt)
{
    GameLogic gameLogic;
    std::vector<Types::Turn> bestMoves;
    float bestValue = -std::numeric_limits<float>::infinity();

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
                    std::string capturedPiece = chessboard.getPiece(move);
                    float moveValue = 0;

                    // If a piece is captured, add its value
                    if (capturedPiece != "---")
                    {
                        moveValue += pieceValues.at(capturedPiece[1]);
                    }

                    // If this move is better than the current best, clear the list and add this move
                    if (moveValue > bestValue)
                    {
                        bestValue = moveValue;
                        bestMoves.clear();
                        bestMoves.push_back({turn,
                                             player,
                                             currentSquare,
                                             move,
                                             piece,
                                             capturedPiece});
                    }
                    // If this move is equal to the current best, add it to the list
                    else if (moveValue == bestValue)
                    {
                        bestMoves.push_back({turn,
                                             player,
                                             currentSquare,
                                             move,
                                             piece,
                                             capturedPiece});
                    }
                }
            }
        }
    }

    // Choose a random move from the best moves
    if (!bestMoves.empty())
    {
        std::uniform_int_distribution<> dist(0, bestMoves.size() - 1);
        int randomIndex = dist(rng);
        return bestMoves[randomIndex];
    }

    // No legal moves available
    throw std::runtime_error("No legal moves available for AI player");
}

Types::Turn AI::getMinMaxAIMove(const char player, int turn, bool alt, int depth)
{
    GameLogic gameLogic;
    std::vector<Types::Turn> bestMoves;
    float bestValue = -std::numeric_limits<float>::infinity();

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
                    // Make the move
                    std::string capturedPiece = chessboard.getPiece(move);
                    chessboard.setCell(move, piece);
                    chessboard.setCell(currentSquare, "---");

                    // Evaluate the move
                    float moveValue = -minMax(gameLogic, (player == 'w' ? 'b' : 'w'), turn + 1, alt, depth - 1, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

                    // Undo the move
                    chessboard.setCell(currentSquare, piece);
                    chessboard.setCell(move, capturedPiece);

                    // If this move is better than the current best, clear the list and add this move
                    if (moveValue > bestValue)
                    {
                        bestValue = moveValue;
                        bestMoves.clear();
                        bestMoves.push_back({turn,
                                             player,
                                             currentSquare,
                                             move,
                                             piece,
                                             capturedPiece});
                    }
                    // If this move is equal to the current best, add it to the list
                    else if (moveValue == bestValue)
                    {
                        bestMoves.push_back({turn,
                                             player,
                                             currentSquare,
                                             move,
                                             piece,
                                             capturedPiece});
                    }
                }
            }
        }
    }

    // Choose a random move from the best moves
    if (!bestMoves.empty())
    {
        std::uniform_int_distribution<> dist(0, bestMoves.size() - 1);
        int randomIndex = dist(rng);
        return bestMoves[randomIndex];
    }

    // No legal moves available
    throw std::runtime_error("No legal moves available for AI player");
}

float AI::minMax(GameLogic &gameLogic, char player, int turn, bool alt, int depth, float alpha, float beta)
{
    if (depth == 0)
    {
        return evaluateBoard();
    }

    float bestValue = (player == 'w') ? -std::numeric_limits<float>::infinity() : std::numeric_limits<float>::infinity();

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
                    // Make the move
                    std::string capturedPiece = chessboard.getPiece(move);
                    chessboard.setCell(move, piece);
                    chessboard.setCell(currentSquare, "---");

                    float value = minMax(gameLogic, (player == 'w' ? 'b' : 'w'), turn + 1, alt, depth - 1, alpha, beta);

                    // Undo the move
                    chessboard.setCell(currentSquare, piece);
                    chessboard.setCell(move, capturedPiece);

                    if (player == 'w')
                    {
                        bestValue = std::max(bestValue, value);
                        alpha = std::max(alpha, bestValue);
                    }
                    else
                    {
                        bestValue = std::min(bestValue, value);
                        beta = std::min(beta, bestValue);
                    }

                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }
    }

    return bestValue;
}

float AI::evaluateBoard()
{
    float score = 0;
    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Coord currentSquare = {col, row};
            std::string piece = chessboard.getPiece(currentSquare);
            if (piece != "---")
            {
                float pieceValue = pieceValues.at(piece[1]);
                score += (piece[0] == 'w') ? pieceValue : -pieceValue;
            }
        }
    }
    return score;
}
