#include "include/logic.h"
#include "include/globals.h"

std::vector<Types::Coord> Logic::getPawnMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getRookMoves(Types::Coord coord, char player)
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
        std::cout << newY << std::endl;
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

std::vector<Types::Coord> Logic::getTaliaMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getElephantMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getVizierMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    std::vector<Types::Coord> possibleMoves = {
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

std::vector<Types::Coord> Logic::getKhanMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getWarEngineMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getAdminMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getMongolMoves(Types::Coord coord, char player)
{
    Chessboard chessboard;
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

std::vector<Types::Coord> Logic::getCamelMoves(Types::Coord coord, char player)
{
    Chessboard chessboard;
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

std::vector<Types::Coord> calcGiraffePath(Types::Coord coord, std::vector<int> modifer, char player, char enemy)
{
    Chessboard chessboard;
    std::vector<Types::Coord> moves;
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        { // y  coords
            if (modifer[i] > 0)
            { // up
                int spaceUp = coord.y;
                for (int j = 0; j < spaceUp; j++)
                {
                    Types::Coord newCoord = {coord.x, coord.y + j};
                    std::string target = chessboard.getPiece(newCoord);
                    if (j < 2 && target != "---")
                    {
                        break;
                    }
                    else
                    {
                        if (target[0] == player)
                        {
                            break;
                        }
                        moves.push_back(newCoord);
                        if (target[0] == enemy)
                        {
                            break;
                        }
                    }
                }
            }
            else
            { // down
                int spaceDown = Chessboard::rows - coord.y - 1;
                for (int j = 0; j < spaceDown; j++)
                {
                    Types::Coord newCoord = {coord.x, coord.y - j};
                    std::string target = chessboard.getPiece(newCoord);
                    if (j < 2 && target != "---")
                    {
                        break;
                    }
                    else
                    {
                        if (target[0] == player)
                        {
                            break;
                        }
                        moves.push_back(newCoord);
                        if (target[0] == enemy)
                        {
                            break;
                        }
                    }
                }
            }
        }
        else
        { // x coords
            if (modifer[i] > 0)
            { // right
                int spaceRight = Chessboard::rows - coord.x;
                for (int j = 0; j < spaceRight; j++)
                {
                    Types::Coord newCoord = {coord.x + j, coord.y};
                    std::string target = chessboard.getPiece(newCoord);
                    if (j < 2 && target != "---")
                    {
                        break;
                    }
                    else
                    {
                        if (target[0] == player)
                        {
                            break;
                        }
                        moves.push_back(newCoord);
                        if (target[0] == enemy)
                        {
                            break;
                        }
                    }
                }
            }
            else
            { // left
                int spaceLeft = coord.x;
                for (int j = 0; j < spaceLeft; j++)
                {
                    Types::Coord newCoord = {coord.x - j, coord.y};
                    std::string target = chessboard.getPiece(newCoord);
                    if (j < 2 && target != "---")
                    {
                        break;
                    }
                    else
                    {
                        if (target[0] == player)
                        {
                            break;
                        }
                        moves.push_back(newCoord);
                        if (target[0] == enemy)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    return moves;
}

std::vector<Types::Coord> Logic::getGiraffeMoves(Types::Coord coord, char player)
{
    Chessboard chessboard;
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    // giraffes can be blocked so their paths need to be tracked very carefully
    // each giraffe begins by moving one space diagonally
    std::vector<Types::Coord> possibleMoves = {
        {coord.x + 1, coord.y + 1},
        {coord.x + 1, coord.y - 1},
        {coord.x - 1, coord.y + 1},
        {coord.x - 1, coord.y - 1}};
    // from this place, each peice can move at minimum 3 spaces in the relevant x/y
    // axis for that place. for example, if the diagonal spot is up and to the left,
    // the giraffe can move a minimum of 3 spaces from the relecant diagonal place
    // either up or to the left
    std::vector<std::vector<int>> modifers = {
        {1, 1},
        {1, -1},
        {-1, +1},
        {-1, -1}};

    for (int i = 0; i < 4; i++)
    {
        // std::vector<Types::Coord> newMoves = calcGiraffePath(possibleMoves[i], modifers[i], player, enemy);
        //  moves.insert(moves.end(), newMoves.begin(), newMoves.end());
        std::cout << i << ": " << std::endl; /// newMoves[0].x << "," << newMoves[0].y << std::endl;
    }

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
