// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <iostream>
#include <vector>
#include "pieceLogic.h"
#include "globals.h"

// move logic
std::vector<Types::Coord> PieceLogic::getPawnMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord forwardMove = {coord.x, coord.y + direction};
    if (forwardMove.y >= 0 && forwardMove.y < Chessboard::cols &&
        chessboard.getPiece(forwardMove) == "---")
    {
        moves.push_back(forwardMove);
    }

    Types::Coord leftCapture = {coord.x - 1, coord.y + direction};
    if (leftCapture.x >= 0 && leftCapture.x < Chessboard::rows &&
        leftCapture.y >= 0 && leftCapture.y < Chessboard::cols &&
        chessboard.getPiece(leftCapture).color() == enemy)
    {
        moves.push_back(leftCapture);
    }

    Types::Coord rightCapture = {coord.x + 1, coord.y + direction};
    if (rightCapture.x >= 0 && rightCapture.x <= Chessboard::rows &&
        rightCapture.y >= 0 && rightCapture.y < Chessboard::cols &&
        chessboard.getPiece(rightCapture).color() == enemy)
    {
        moves.push_back(rightCapture);
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getRookMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    // Check all orthogonal directions
    for (const auto &direction : ORTHOGONAL)
    {
        for (int i = 1; i < Chessboard::rows; ++i)
        {
            Types::Coord newCoord = {coord.x + (direction.x * i),
                                     coord.y + (direction.y * i)};

            if (!chessboard.isValidCoord(newCoord))
            {
                break;
            }

            Types::Piece target = chessboard.getPiece(newCoord);
            if (target == "---")
            {
                moves.push_back(newCoord);
            }
            else if (target.color() == enemy)
            {
                moves.push_back(newCoord);
                break;
            }
            else
            {
                break;
            }
        }
    }
    return moves;
}

std::vector<Types::Coord> PieceLogic::getTaliaMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    // Check all diagonal directions
    for (const auto &direction : DIAGONAL)
    {
        for (int i = 1; i < Chessboard::rows; ++i)
        {
            Types::Coord newCoord = {coord.x + (direction.x * i),
                                     coord.y + (direction.y * i)};
            Types::Piece target = chessboard.getPiece(newCoord);
            // If it's the first move, only check if the target is empty.
            // Talia cannot move one square and is blocked by any piece
            if (i == 1)
            {
                if (target != "---")
                {
                    break;
                }
            }
            else
            {
                if (target == "---")
                {
                    moves.push_back(newCoord);
                }
                else if (target.color() == enemy)
                {
                    moves.push_back(newCoord);
                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moves;
}

std::vector<Types::Coord> PieceLogic::getElephantMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    std::vector<Types::Coord> possibleMoves = {
        // down left
        {coord.x - 2, coord.y + 2},
        // down right
        {coord.x + 2, coord.y + 2},
        // up left
        {coord.x - 2, coord.y - 2},
        // up right
        {coord.x + 2, coord.y - 2}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getVizierMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    for (const auto &direction : DIAGONAL)
    {
        Types::Coord move = {coord.x + direction.x, coord.y + direction.y};
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getKhanMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    for (const auto &direction : ALL_DIRECTIONS)
    {
        Types::Coord move = {coord.x + direction.x, coord.y + direction.y};
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getWarEngineMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    std::vector<Types::Coord> possibleMoves = {
        // up
        {coord.x, coord.y - 2},
        // down
        {coord.x, coord.y + 2},
        // left
        {coord.x - 2, coord.y},
        // right
        {coord.x + 2, coord.y},
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAdminMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    for (const auto &direction : ORTHOGONAL)
    {
        Types::Coord move = {coord.x + direction.x, coord.y + direction.y};
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getMongolMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    std::vector<Types::Coord> possibleMoves = {
        {coord.x + 1, coord.y + 2},
        {coord.x + 1, coord.y - 2},
        {coord.x - 1, coord.y + 2},
        {coord.x - 1, coord.y - 2},
        {coord.x + 2, coord.y + 1},
        {coord.x + 2, coord.y - 1},
        {coord.x - 2, coord.y + 1},
        {coord.x - 2, coord.y - 1}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getCamelMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    std::vector<Types::Coord> possibleMoves = {
        {coord.x + 1, coord.y + 3},
        {coord.x + 1, coord.y - 3},
        {coord.x - 1, coord.y + 3},
        {coord.x - 1, coord.y - 3},
        {coord.x + 3, coord.y + 1},
        {coord.x + 3, coord.y - 1},
        {coord.x - 3, coord.y + 1},
        {coord.x - 3, coord.y - 1}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getGiraffeMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    char enemy = (player == 'w') ? 'b' : 'w';

    for (auto &dir : directions)
    {
        int dx = dir[0], dy = dir[1];
        Types::Coord diagonalMove = {coord.x + dx, coord.y + dy};

        // Check if the one square diagonal move is valid and empty
        if (!chessboard.isValidCoord(diagonalMove) ||
            chessboard.getPiece(diagonalMove) != "---")
            continue;

        // Check if the immediate horizontal and vertical moves are blocked
        bool blockedHorizontal = false, blockedVertical = false;

        for (int i = 1; i < 2; ++i)
        {
            Types::Coord immediateH = {diagonalMove.x + i * dx, diagonalMove.y};
            Types::Coord immediateV = {diagonalMove.x, diagonalMove.y + i * dy};

            if (chessboard.isValidCoord(immediateH) &&
                chessboard.getPiece(immediateH) != "---")
            {
                blockedHorizontal = true;
            }
            if (chessboard.isValidCoord(immediateV) &&
                chessboard.getPiece(immediateV) != "---")
            {
                blockedVertical = true;
            }
        }

        // Explore horizontal moves after the diagonalMove
        if (!blockedHorizontal)
        {
            for (int i = 2; i < Chessboard::rows; ++i)
            {
                Types::Coord straightMoveH = {diagonalMove.x +
                                                  i * dx,
                                              diagonalMove.y};
                if (chessboard.isValidCoord(straightMoveH))
                {
                    Types::Piece targetH = chessboard.getPiece(straightMoveH);
                    if (targetH == "---")
                    {
                        moves.push_back(straightMoveH);
                    }
                    else if (targetH.color() == enemy)
                    {
                        moves.push_back(straightMoveH);
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        // Explore vertical moves after the diagonalMove
        if (!blockedVertical)
        {
            for (int i = 2; i < Chessboard::rows; ++i)
            {
                Types::Coord straightMoveV = {diagonalMove.x,
                                              diagonalMove.y + i * dy};
                if (chessboard.isValidCoord(straightMoveV))
                {
                    Types::Piece targetV = chessboard.getPiece(straightMoveV);
                    if (targetV == "---")
                    {
                        moves.push_back(straightMoveV);
                    }
                    else if (targetV.color() == enemy)
                    {
                        moves.push_back(straightMoveV);
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    return moves;
}

// alternative move logic
std::vector<Types::Coord> PieceLogic::getAltPawnMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';

    // Check if it's the pawn's first move
    bool isFirstMove = (player == 'w' && coord.y == 7) ||
                       (player == 'b' && coord.y == 2);

    // Single move forward
    Types::Coord forwardMove = {coord.x, coord.y + direction};
    if (forwardMove.y >= 0 && forwardMove.y < Chessboard::cols &&
        chessboard.getPiece(forwardMove) == "---")
    {
        moves.push_back(forwardMove);

        // Double move on first move
        if (isFirstMove)
        {
            Types::Coord doubleMove = {coord.x, coord.y + 2 * direction};
            if (doubleMove.y >= 0 && doubleMove.y < Chessboard::cols &&
                chessboard.getPiece(doubleMove) == "---")
            {
                moves.push_back(doubleMove);
            }
        }
    }

    // Captures
    Types::Coord leftCapture = {coord.x - 1, coord.y + direction};
    if (leftCapture.x >= 0 && leftCapture.x < Chessboard::rows &&
        leftCapture.y >= 0 && leftCapture.y < Chessboard::cols &&
        chessboard.getPiece(leftCapture).color() == enemy)
    {
        moves.push_back(leftCapture);
    }

    Types::Coord rightCapture = {coord.x + 1, coord.y + direction};
    if (rightCapture.x >= 0 && rightCapture.x < Chessboard::rows &&
        rightCapture.y >= 0 && rightCapture.y < Chessboard::cols &&
        chessboard.getPiece(rightCapture).color() == enemy)
    {
        moves.push_back(rightCapture);
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltWarEngineMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    // Possible moves for alt war engine:
    // - Orthogonal moves: 2 squares up/down/left/right
    // - Diagonal moves: 2 squares in each diagonal direction
    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 2},
        {coord.x, coord.y + 2},
        {coord.x - 2, coord.y},
        {coord.x + 2, coord.y},
        {coord.x + 2, coord.y - 2},
        {coord.x - 2, coord.y + 2},
        {coord.x - 2, coord.y - 2},
        {coord.x + 2, coord.y + 2}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltElephantMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    // Possible moves for alt elephant:
    // Orthogonal moves: 1 square up/down/left/right
    // Diagonal moves: 2 squares in each diagonal direction
    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1},
        {coord.x, coord.y + 1},
        {coord.x - 1, coord.y},
        {coord.x + 1, coord.y},
        {coord.x - 2, coord.y + 2},
        {coord.x + 2, coord.y + 2},
        {coord.x - 2, coord.y - 2},
        {coord.x + 2, coord.y - 2}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltVizierMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    // Possible moves for alt vizier:
    // 1 square diagonally in all directions
    // 2 squares diagonally in all directions
    std::vector<Types::Coord> possibleMoves = {
        {coord.x - 1, coord.y + 1},
        {coord.x + 1, coord.y + 1},
        {coord.x - 1, coord.y - 1},
        {coord.x + 1, coord.y - 1},
        {coord.x - 2, coord.y + 2},
        {coord.x + 2, coord.y + 2},
        {coord.x - 2, coord.y - 2},
        {coord.x + 2, coord.y - 2}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltAdminMoves(Types::Coord coord, char player)
{
    std::vector<Types::Coord> moves;

    // Possible moves for alt admin:
    // 1 or 2 squares orthogonally in all directions
    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1},
        {coord.x, coord.y + 1},
        {coord.x - 1, coord.y},
        {coord.x + 1, coord.y},
        {coord.x, coord.y - 2},
        {coord.x, coord.y + 2},
        {coord.x - 2, coord.y},
        {coord.x + 2, coord.y}};

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move).color() != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}
