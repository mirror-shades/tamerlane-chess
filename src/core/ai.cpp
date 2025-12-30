// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <string>
#include <cmath>
#include "types.h"
#include "globals.h"
#include "utility.h"
#include "ai.h"

const std::unordered_map<char, float> AI::pieceValues = {
    {'K', 3.5f},
    {'p', 1.0f},
    {'M', 3.0f},
    {'C', 2.0f},
    {'G', 4.0f},
    {'R', 5.0f},
    {'T', 2.5f},
    {'E', 1.5f},
    {'W', 2.0f},
    {'V', 1.5f},
    {'A', 1.5f}};

// Helper function to round to 2 decimal places
static float roundToTwoDecimals(float value)
{
    return std::round(value * 100.0f) / 100.0f;
}

// Helper function to compare rounded floats with epsilon
static bool roundedFloatsEqual(float a, float b)
{
    const float epsilon = 0.001f; // Small epsilon for comparison
    return std::abs(a - b) < epsilon;
}

Types::Turn AI::minMax(char player,
                       int turn,
                       bool alt,
                       int depth,
                       float alpha,
                       float beta)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Types::Turn> allMoves = generateAllLegalMoves(player, turn, alt);
    if (allMoves.empty())
    {
        throw std::runtime_error("No legal moves available for AI player");
    }

    // Sort moves to evaluate captures first
    std::sort(allMoves.begin(), allMoves.end(),
              [](const Types::Turn &a, const Types::Turn &b)
              { return a.pieceCaptured != "---" && b.pieceCaptured == "---"; });

    std::vector<Types::Turn> bestMoves;
    float bestRoundedValue = (player == 'w') ? -std::numeric_limits<float>::infinity()
                                            : std::numeric_limits<float>::infinity();
    float bestValue = (player == 'w') ? -std::numeric_limits<float>::infinity()
                                      : std::numeric_limits<float>::infinity();

    GameLogic gameLogic;
    for (const auto &move : allMoves)
    {
        // Make move
        std::string originalPiece = chessboard.getPiece(move.finalSquare).toString();
        chessboard.setCell(move.finalSquare, move.pieceMoved);
        chessboard.setCell(move.initialSquare, "---");

        // Evaluate position
        float value;
        if (depth <= 1)
        {
            value = evaluateBoard();
        }
        else
        {
            value = minMaxHelper(gameLogic,
                                 (player == 'w' ? 'b' : 'w'),
                                 turn + 1,
                                 alt,
                                 depth - 1,
                                 alpha,
                                 beta);
        }

        // Undo move
        chessboard.setCell(move.initialSquare, move.pieceMoved);
        chessboard.setCell(move.finalSquare, originalPiece);

        // Round value to 2 decimal places for comparison
        float roundedValue = roundToTwoDecimals(value);

        // Update best moves based on rounded value
        if (player == 'w')
        {
            if (roundedValue > bestRoundedValue)
            {
                bestRoundedValue = roundedValue;
                bestValue = value;
                bestMoves.clear();
                bestMoves.push_back(move);
            }
            else if (roundedFloatsEqual(roundedValue, bestRoundedValue))
            {
                bestMoves.push_back(move);
            }
            alpha = std::max(alpha, value);
        }
        else
        {
            if (roundedValue < bestRoundedValue)
            {
                bestRoundedValue = roundedValue;
                bestValue = value;
                bestMoves.clear();
                bestMoves.push_back(move);
            }
            else if (roundedFloatsEqual(roundedValue, bestRoundedValue))
            {
                bestMoves.push_back(move);
            }
            beta = std::min(beta, value);
        }

        if (beta <= alpha)
        {
            break;
        }
    }

    // Randomly select from tied moves
    if (bestMoves.empty())
    {
        // Fallback: should never happen, but use first move if it does
        bestMoves.push_back(allMoves[0]);
    }
    std::uniform_int_distribution<size_t> dist(0, bestMoves.size() - 1);
    Types::Turn bestMove = bestMoves[dist(rng)];

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "AI move calculation time: " << elapsed.count() << " seconds"
              << std::endl;

    bestMove.score = bestValue;
    return bestMove;
}

float AI::minMaxHelper(GameLogic gameLogic,
                       char player,
                       int turn,
                       bool alt,
                       int depth,
                       float alpha,
                       float beta)
{
    if (depth == 0)
        return evaluateBoard();

    float bestValue = (player == 'w') ? -std::numeric_limits<float>::infinity()
                                      : std::numeric_limits<float>::infinity();

    std::vector<Types::Turn> allMoves = generateAllLegalMoves(player,
                                                              turn, alt);

    std::sort(allMoves.begin(), allMoves.end(),
              [](const Types::Turn &a, const Types::Turn &b)
              { return a.pieceCaptured != "---" && b.pieceCaptured == "---"; });

    const int maxMovesToConsider = std::min(static_cast<int>(
                                                allMoves.size()),
                                            5);

    for (int i = 0; i < maxMovesToConsider; ++i)
    {
        const auto &moveInfo = allMoves[i];
        chessboard.setCell(moveInfo.finalSquare, moveInfo.pieceMoved);
        chessboard.setCell(moveInfo.initialSquare, "---");

        float value = minMaxHelper(gameLogic,
                                   (player == 'w' ? 'b' : 'w'),
                                   turn + 1,
                                   alt,
                                   depth - 1,
                                   alpha,
                                   beta);

        chessboard.setCell(moveInfo.initialSquare, moveInfo.pieceMoved);
        chessboard.setCell(moveInfo.finalSquare, moveInfo.pieceCaptured);

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
            break;
    }

    return bestValue;
}

std::vector<Types::Turn> AI::generateAllLegalMoves(char player,
                                                   int turn,
                                                   bool alt)
{
    GameLogic gameLogic;
    std::vector<Types::Turn> allMoves;
    allMoves.reserve(100);

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Coord currentSquare = {col, row};
            std::string piece = chessboard.getPiece(currentSquare).toString();

            if (piece[0] == player)
            {
                auto possibleMoves = gameLogic.getMoves(currentSquare,
                                                        piece,
                                                        player,
                                                        alt);
                auto legalMoves = gameLogic.filterLegalMoves(possibleMoves,
                                                             currentSquare,
                                                             piece,
                                                             player,
                                                             alt);

                for (const auto &move : legalMoves)
                {
                    allMoves.emplace_back(Types::Turn{turn,
                                                      player,
                                                      currentSquare,
                                                      move,
                                                      piece,
                                                      chessboard.getPiece(move),
                                                      0.0f});
                }
            }
        }
    }

    return allMoves;
}

float AI::evaluateBoard()
{
    float score = 0;
    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = chessboard.getPiece({col, row}).toString();
            if (piece != "---")
            {
                float pieceValue = pieceValues.at(piece[1]);
                score += (piece[0] == 'w') ? pieceValue : -pieceValue;

                // Add positional evaluation
                score += evaluatePosition(piece, col, row);
            }
        }
    }
    return score;
}

float AI::evaluatePosition(const std::string &piece, int col, int row)
{
    float positionScore = 0;
    char pieceType = piece[1];
    bool isWhite = (piece[0] == 'w');

    // Evaluate pawn structure
    if (pieceType == 'p')
    {
        positionScore += evaluatePawnStructure(col, row, isWhite);
    }

    // Evaluate piece mobility
    positionScore += evaluatePieceMobility(piece, col, row);

    // Evaluate king safety
    if (pieceType == 'K')
    {
        positionScore += evaluateKingSafety(col, row, isWhite);
    }

    // Evaluate control of the center
    positionScore += evaluateCenterControl(col, row);

    return isWhite ? positionScore : -positionScore;
}

float AI::evaluatePawnStructure(int col, int row, bool isWhite)
{
    float score = 0.0f;
    int direction = isWhite ? 1 : -1;

    // Check for doubled pawns
    for (int r = 0; r < Chessboard::rows; ++r)
    {
        if (r != row &&
            chessboard.getPiece({col, r}) == (isWhite ? "wp" : "bp"))
        {
            // Penalty for doubled pawns
            score -= 0.5f;
        }
    }

    // Check for isolated pawns
    bool isolated = true;
    for (int c = std::max(0, col - 1);
         c <= std::min(Chessboard::cols - 1, col + 1);
         ++c)
    {
        if (c != col)
        {
            for (int r = 0; r < Chessboard::rows; ++r)
            {
                if (chessboard.getPiece({c, r}) == (isWhite ? "wp" : "bp"))
                {
                    isolated = false;
                    break;
                }
            }
        }
        if (!isolated)
            break;
    }
    if (isolated)
        // Penalty for isolated pawns
        score -= 0.3f;

    // Bonus for advanced pawns
    int advancementBonus = isWhite ? row : (Chessboard::rows - 1 - row);
    score += 0.1f * advancementBonus;

    // Check for pawn chains
    if (col > 0 && row + direction >= 0 && row + direction < Chessboard::rows)
    {
        if (chessboard.getPiece({col - 1, row + direction}) ==
            (isWhite ? "wp" : "bp"))
        {
            // Bonus for being part of a pawn chain
            score += 0.2f;
        }
    }

    return score;
}

float AI::evaluatePieceMobility(const std::string &piece, int col, int row)
{
    GameLogic gameLogic;
    float mobilityScore = 0.0f;
    Types::Coord currentSquare = {col, row};
    char player = piece[0];
    // Assuming standard moves, not alternate moves
    bool alt = false;

    auto possibleMoves = gameLogic.getMoves(currentSquare,
                                            piece,
                                            player,
                                            alt);
    auto legalMoves = gameLogic.filterLegalMoves(possibleMoves,
                                                 currentSquare,
                                                 piece,
                                                 player,
                                                 alt);

    // Base mobility score on the number of legal moves
    mobilityScore += legalMoves.size() * 0.1f;

    // Additional bonuses for specific pieces
    switch (piece[1])
    {
    case 'R':
        // Rooks benefit more from open lines
        mobilityScore += legalMoves.size() * 0.05f;
        break;
    case 'G':
        // Giraffe
        // Girrafe benefit from open lines and fewer pieces on the board
        mobilityScore += legalMoves.size() * 0.05f;
        break;
    case 'E':
        // Elephant
        // Long-range pieces benefit from more options
        mobilityScore += legalMoves.size() * 0.03f;
        break;
    case 'M':
        // Knights benefit from central positions
        mobilityScore += legalMoves.size() * 0.04f;
        break;
    case 'C':
        // Camel (3x1 Knight)
        // Camels benefit from central positions
        mobilityScore += legalMoves.size() * 0.03f;
        break;
    case 'T':
        // Taliah (Restricted Bishop)
        // Taliahs benefit from open diagonals
        mobilityScore += legalMoves.size() * 0.035f;
        break;
    case 'W':
        // War Machines benefit from open lines
        mobilityScore += legalMoves.size() * 0.04f;
        break;
    case 'p':
        // Pawns benefit more from forward moves
        for (const auto &move : legalMoves)
        {
            if ((player == 'w' && move.y > row) ||
                (player == 'b' && move.y < row))
            {
                mobilityScore += 0.15f;
            }
        }
        break;
    case 'K':
        // King mobility is less important in the midgame/endgame
        mobilityScore += legalMoves.size() * 0.01f;
        break;
    }

    return mobilityScore;
}

float AI::evaluateKingSafety(int col, int row, bool isWhite)
{
    float safetyScore = 0.0f;
    char player = isWhite ? 'w' : 'b';
    char opponent = isWhite ? 'b' : 'w';

    // Check pawn shield
    int pawnShieldDirection = isWhite ? -1 : 1;
    for (int i = -1; i <= 1; ++i)
    {
        if (col + i >= 0 && col + i < Chessboard::cols)
        {
            // ugliest variable ever written but linter+prettier are killing me
            std::string piece =
                chessboard.getPiece({col +
                                         i,
                                     row +
                                         pawnShieldDirection})
                    .toString();
            if (piece[0] == player && piece[1] == 'p')
            {
                safetyScore += 0.5f;
            }
        }
    }

    // Check nearby friendly pieces
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            if (col + i >= 0 && col + i < Chessboard::cols &&
                row + j >= 0 && row + j < Chessboard::rows)
            {
                std::string piece = chessboard.getPiece({col + i, row + j})
                                        .toString();
                if (piece[0] == player)
                {
                    safetyScore += 0.2f;
                }
            }
        }
    }

    // Check opponent's piece proximity
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            if (col + i >= 0 && col + i < Chessboard::cols &&
                row + j >= 0 && row + j < Chessboard::rows)
            {
                std::string piece = chessboard.getPiece({col + i, row + j})
                                        .toString();
                if (piece[0] == opponent)
                {
                    safetyScore -= 0.3f;
                    // Additional penalty for specific threatening pieces

                    switch (piece[1])
                    {
                    case 'R':
                    case 'G':
                    case 'E':
                        safetyScore -= 0.2f;
                        break;
                    case 'M':
                    case 'C':
                    case 'W':
                        safetyScore -= 0.15f;
                        break;
                    }
                }
            }
        }
    }

    return safetyScore;
}

float AI::evaluateCenterControl(int col, int row)
{
    float centerControlScore = 0.0f;

    // Define the center area for Tamerlane Chess (10x11 board)
    const int centerStartCol = 3;
    const int centerEndCol = 6;
    const int centerStartRow = 3;
    const int centerEndRow = 7;

    // Check if the piece is in the center area
    if (col >= centerStartCol && col <= centerEndCol &&
        row >= centerStartRow && row <= centerEndRow)
    {
        // Assign a base score for being in the center
        centerControlScore += 0.5f;

        // Additional score based on how close to the absolute center
        int distanceFromCenterCol = std::min(col - centerStartCol, centerEndCol - col);
        int distanceFromCenterRow = std::min(row - centerStartRow, centerEndRow - row);
        centerControlScore += 0.1f * (3 - distanceFromCenterCol) +
                              0.1f * (4 - distanceFromCenterRow);

        // Bonus for controlling the absolute center (4,5) and (5,5)
        if ((col == 4 || col == 5) && row == 5)
        {
            centerControlScore += 0.3f;
        }
    }

    // Evaluate control of important central files (columns)
    if (col >= 3 && col <= 6)
    {
        centerControlScore += 0.2f;
    }

    // Evaluate control of important ranks (rows) for Tamerlane Chess
    if (row >= 4 && row <= 6)
    {
        centerControlScore += 0.2f;
    }

    return centerControlScore;
}

float AI::quiescenceSearch(char player, float alpha, float beta, int maxDepth)
{
    float standPat = evaluateBoard();

    if (player == 'w')
        alpha = std::max(alpha, standPat);
    else
        beta = std::min(beta, standPat);

    if (beta <= alpha)
        return standPat;

    if (maxDepth == 0)
        return standPat;

    std::vector<Types::Turn> captureMoves =
        generateCaptureMoves(player);

    for (const auto &move : captureMoves)
    {
        chessboard.setCell(move.finalSquare, move.pieceMoved);
        chessboard.setCell(move.initialSquare, "---");

        float score = -quiescenceSearch(player == 'w' ? 'b' : 'w',
                                        -beta,
                                        -alpha,
                                        maxDepth - 1);

        chessboard.setCell(move.initialSquare, move.pieceMoved);
        chessboard.setCell(move.finalSquare, move.pieceCaptured);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    return alpha;
}

std::vector<Types::Turn> AI::generateCaptureMoves(char player)
{
    std::vector<Types::Turn> captureMoves;
    GameLogic gameLogic;

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Coord currentSquare = {col, row};
            std::string piece = chessboard.getPiece(currentSquare).toString();

            if (piece[0] == player)
            {
                auto possibleMoves = gameLogic.getMoves(currentSquare,
                                                        piece,
                                                        player,
                                                        false);
                auto legalMoves = gameLogic.filterLegalMoves(possibleMoves,
                                                             currentSquare,
                                                             piece,
                                                             player,
                                                             false);

                for (const auto &move : legalMoves)
                {
                    std::string capturedPiece =
                        chessboard.getPiece(move).toString();
                    if (capturedPiece != "---" && capturedPiece[0] != player)
                    {
                        // Initialize all fields including score
                        Types::Turn turn = {
                            0,             // turn
                            player,        // player
                            currentSquare, // initialSquare
                            move,          // finalSquare
                            piece,         // pieceMoved
                            capturedPiece, // pieceCaptured
                            0.0f           // score
                        };
                        captureMoves.push_back(turn);
                    }
                }
            }
        }
    }

    return captureMoves;
}
