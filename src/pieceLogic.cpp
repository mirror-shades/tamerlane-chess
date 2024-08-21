#include <iostream>
#include "include/pieceLogic.h"
#include "include/globals.h"

// move logic
std::vector<Types::Coord> PieceLogic::getPawnMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord forwardMove = {coord.x, coord.y + direction};
    if (forwardMove.y >= 0 && forwardMove.y < Chessboard::cols && chessboard.getPiece(forwardMove) == "---")
    {
        moves.push_back(forwardMove);
    }

    Types::Coord leftCapture = {coord.x - 1, coord.y + direction};
    if (leftCapture.x >= 0 && leftCapture.x < Chessboard::rows &&
        leftCapture.y >= 0 && leftCapture.y < Chessboard::cols &&
        chessboard.getPiece(leftCapture)[0] == enemy)
    {
        moves.push_back(leftCapture);
    }

    Types::Coord rightCapture = {coord.x + 1, coord.y + direction};
    if (rightCapture.x >= 0 && rightCapture.x <= Chessboard::rows &&
        rightCapture.y >= 0 && rightCapture.y < Chessboard::cols &&
        chessboard.getPiece(rightCapture)[0] == enemy)
    {
        moves.push_back(rightCapture);
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getRookMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';
    int spaceDown = Chessboard::rows - coord.y - 1;
    int spaceUp = coord.y;
    int spaceRight = Chessboard::rows - coord.x;
    int spaceLeft = coord.x;
    for (int i = 0; i < spaceDown; i++)
    {
        int newY = coord.y + i + 1;
        Types::Coord newCoord = {coord.x, newY};
        std::string target = chessboard.getPiece(newCoord);
        if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < spaceUp; i++)
    {
        int newY = coord.y - i - 1;
        Types::Coord newCoord = {coord.x, newY};
        std::string target = chessboard.getPiece(newCoord);
        if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < spaceRight; i++)
    {
        int newX = coord.x + i + 1;
        Types::Coord newCoord = {newX, coord.y};
        std::string target = chessboard.getPiece(newCoord);
        if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < spaceLeft; i++)
    {
        int newX = coord.x - i - 1;
        Types::Coord newCoord = {newX, coord.y};
        std::string target = chessboard.getPiece(newCoord);
        if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getTaliaMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    // up right
    for (int i = 1; coord.x + i <= Chessboard::rows && coord.y - i >= 0; ++i)
    {
        int newX = coord.x + i;
        int newY = coord.y - i;
        Types::Coord newCoord = {newX, newY};
        std::string target = chessboard.getPiece(newCoord);

        if (i == 1)
        {
            if (target != "---")
            {
                break;
            }
        }
        else if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }

    // up left
    for (int i = 1; coord.x - i >= 0 && coord.y - i >= 0; ++i)
    {
        int newX = coord.x - i;
        int newY = coord.y - i;
        Types::Coord newCoord = {newX, newY};
        std::string target = chessboard.getPiece(newCoord);

        if (i == 1)
        {
            if (target != "---")
            {
                break;
            }
        }
        else if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }

    // down right
    for (int i = 1; coord.x + i <= Chessboard::rows && coord.y + i < Chessboard::rows; ++i)
    {
        int newX = coord.x + i;
        int newY = coord.y + i;
        Types::Coord newCoord = {newX, newY};
        std::string target = chessboard.getPiece(newCoord);

        if (i == 1)
        {
            if (target != "---")
            {
                break;
            }
        }
        else if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }

    // down left
    for (int i = 1; coord.x - i >= 0 && coord.y + i < Chessboard::rows; ++i)
    {
        int newX = coord.x - i;
        int newY = coord.y + i;
        Types::Coord newCoord = {newX, newY};
        std::string target = chessboard.getPiece(newCoord);

        if (i == 1)
        {
            if (target != "---")
            {
                break;
            }
        }
        else if (target == "---")
        {
            moves.push_back(newCoord);
        }
        else if (target[0] == enemy)
        {
            moves.push_back(newCoord);
            break;
        }
        else
        {
            break;
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getElephantMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x - 2, coord.y + 2}, // down left
        {coord.x + 2, coord.y + 2}, // down right
        {coord.x - 2, coord.y - 2}, // up left
        {coord.x + 2, coord.y - 2}  // up right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getVizierMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x - 1, coord.y + 1}, // left down
        {coord.x + 1, coord.y + 1}, // right down
        {coord.x - 1, coord.y - 1}, // left up
        {coord.x + 1, coord.y - 1}  // right up
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getKhanMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1},     // up
        {coord.x, coord.y + 1},     // down
        {coord.x - 1, coord.y},     // left
        {coord.x + 1, coord.y},     // right
        {coord.x - 1, coord.y + 1}, // down left
        {coord.x + 1, coord.y + 1}, // down right
        {coord.x - 1, coord.y - 1}, // up left
        {coord.x + 1, coord.y - 1}  // up right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getWarEngineMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 2}, // up
        {coord.x, coord.y + 2}, // down
        {coord.x - 2, coord.y}, // left
        {coord.x + 2, coord.y}, // right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAdminMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1}, // up
        {coord.x, coord.y + 1}, // down
        {coord.x - 1, coord.y}, // left
        {coord.x + 1, coord.y}, // right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getMongolMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

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
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getCamelMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

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
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getGiraffeMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    char enemy = (player == 'w') ? 'b' : 'w';

    for (auto &dir : directions)
    {
        int dx = dir[0], dy = dir[1];
        Types::Coord diagonalMove = {coord.x + dx, coord.y + dy};

        // Check if the one square diagonal move is valid and empty
        if (!chessboard.isValidCoord(diagonalMove) || chessboard.getPiece(diagonalMove) != "---")
            continue;

        // Check if the immediate horizontal and vertical moves are blocked
        bool blockedHorizontal = false, blockedVertical = false;

        for (int i = 1; i < 2; ++i)
        {
            Types::Coord immediateH = {diagonalMove.x + i * dx, diagonalMove.y};
            Types::Coord immediateV = {diagonalMove.x, diagonalMove.y + i * dy};

            if (chessboard.isValidCoord(immediateH) && chessboard.getPiece(immediateH) != "---")
            {
                blockedHorizontal = true;
            }
            if (chessboard.isValidCoord(immediateV) && chessboard.getPiece(immediateV) != "---")
            {
                blockedVertical = true;
            }
        }

        // Function to add possible moves if path is not blocked
        auto addMoves = [&](Types::Coord move, bool blocked)
        {
            if (!blocked && chessboard.isValidCoord(move))
            {
                std::string target = chessboard.getPiece(move);
                if (target == "---")
                {
                    moves.push_back(move);
                }
                else if (target[0] == enemy)
                {
                    moves.push_back(move);
                }
            }
        };

        // Explore horizontal moves after the diagonalMove
        if (!blockedHorizontal)
        {
            for (int i = 2; i < Chessboard::rows; ++i)
            {
                Types::Coord straightMoveH = {diagonalMove.x + i * dx, diagonalMove.y};
                if (chessboard.isValidCoord(straightMoveH))
                {
                    std::string targetH = chessboard.getPiece(straightMoveH);
                    if (targetH == "---")
                    {
                        moves.push_back(straightMoveH);
                    }
                    else if (targetH[0] == enemy)
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
                Types::Coord straightMoveV = {diagonalMove.x, diagonalMove.y + i * dy};
                if (chessboard.isValidCoord(straightMoveV))
                {
                    std::string targetV = chessboard.getPiece(straightMoveV);
                    if (targetV == "---")
                    {
                        moves.push_back(straightMoveV);
                    }
                    else if (targetV[0] == enemy)
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
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    int direction = (player == 'w') ? -1 : 1;
    char enemy = (player == 'w') ? 'b' : 'w';

    // Check if it's the pawn's first move
    bool isFirstMove = (player == 'w' && coord.y == 7) || (player == 'b' && coord.y == 2);

    // Single move forward
    Types::Coord forwardMove = {coord.x, coord.y + direction};
    if (forwardMove.y >= 0 && forwardMove.y < Chessboard::cols && chessboard.getPiece(forwardMove) == "---")
    {
        moves.push_back(forwardMove);

        // Double move on first move
        if (isFirstMove)
        {
            Types::Coord doubleMove = {coord.x, coord.y + 2 * direction};
            if (doubleMove.y >= 0 && doubleMove.y < Chessboard::cols && chessboard.getPiece(doubleMove) == "---")
            {
                moves.push_back(doubleMove);
            }
        }
    }

    // Captures
    Types::Coord leftCapture = {coord.x - 1, coord.y + direction};
    if (leftCapture.x >= 0 && leftCapture.x < Chessboard::rows &&
        leftCapture.y >= 0 && leftCapture.y < Chessboard::cols &&
        chessboard.getPiece(leftCapture)[0] == enemy)
    {
        moves.push_back(leftCapture);
    }

    Types::Coord rightCapture = {coord.x + 1, coord.y + direction};
    if (rightCapture.x >= 0 && rightCapture.x < Chessboard::rows &&
        rightCapture.y >= 0 && rightCapture.y < Chessboard::cols &&
        chessboard.getPiece(rightCapture)[0] == enemy)
    {
        moves.push_back(rightCapture);
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltWarEngineMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 2},     // up
        {coord.x, coord.y + 2},     // down
        {coord.x - 2, coord.y},     // left
        {coord.x + 2, coord.y},     // right
        {coord.x + 2, coord.y - 2}, // right up
        {coord.x - 2, coord.y + 2}, // left down
        {coord.x - 2, coord.y - 2}, // left up
        {coord.x + 2, coord.y + 2}, // right down
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltElephantMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1},     // up
        {coord.x, coord.y + 1},     // down
        {coord.x - 1, coord.y},     // left
        {coord.x + 1, coord.y},     // right
        {coord.x - 2, coord.y + 2}, // down left
        {coord.x + 2, coord.y + 2}, // down right
        {coord.x - 2, coord.y - 2}, // up left
        {coord.x + 2, coord.y - 2}  // up right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltVizierMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x - 1, coord.y + 1}, // left down
        {coord.x + 1, coord.y + 1}, // right down
        {coord.x - 1, coord.y - 1}, // left up
        {coord.x + 1, coord.y - 1}, // right up
        {coord.x - 2, coord.y + 2}, // left down
        {coord.x + 2, coord.y + 2}, // right down
        {coord.x - 2, coord.y - 2}, // left up
        {coord.x + 2, coord.y - 2}  // right up
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Types::Coord> PieceLogic::getAltAdminMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
        {coord.x, coord.y - 1}, // up
        {coord.x, coord.y + 1}, // down
        {coord.x - 1, coord.y}, // left
        {coord.x + 1, coord.y}, // right
        {coord.x, coord.y - 2}, // up
        {coord.x, coord.y + 2}, // down
        {coord.x - 2, coord.y}, // left
        {coord.x + 2, coord.y}, // right
    };

    for (const auto &move : possibleMoves)
    {
        if (move.x >= 0 && move.x <= Chessboard::rows &&
            move.y >= 0 && move.y < Chessboard::cols - 1 &&
            chessboard.getPiece(move)[0] != player)
        {
            moves.push_back(move);
        }
    }

    return moves;
}
