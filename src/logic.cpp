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

    // Diagonal Top-Right
    for (int i = 1; coord.x + i < Chessboard::rows && coord.y - i >= 0; ++i)
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

    // Diagonal Top-Left
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

    // Diagonal Bottom-Right
    for (int i = 1; coord.x + i < Chessboard::rows && coord.y + i < Chessboard::rows; ++i)
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

    // Diagonal Bottom-Left
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

    Types::Coord upLeft = {coord.x - 2, coord.y - 2};
    if (upLeft.x >= 0 && upLeft.x <= Chessboard::rows &&
        upLeft.y >= 0 && upLeft.y < Chessboard::cols &&
        chessboard.getPiece(upLeft)[0] != player)
    {
        moves.push_back(upLeft);
    }

    Types::Coord upRight = {coord.x + 2, coord.y - 2};
    if (upRight.x >= 0 && upRight.x <= Chessboard::rows &&
        upRight.y >= 0 && upRight.y < Chessboard::cols &&
        chessboard.getPiece(upRight)[0] != player)
    {
        moves.push_back(upRight);
    }

    Types::Coord downLeft = {coord.x - 2, coord.y + 2};
    if (downLeft.x >= 0 && downLeft.x <= Chessboard::rows &&
        downLeft.y >= 0 && downLeft.y < Chessboard::cols - 1 &&
        chessboard.getPiece(downLeft)[0] != player)
    {
        moves.push_back(downLeft);
    }

    Types::Coord downRight = {coord.x + 2, coord.y + 2};
    if (downRight.x >= 0 && downRight.x <= Chessboard::rows &&
        downRight.y >= 0 && downRight.y < Chessboard::cols - 1 &&
        chessboard.getPiece(downRight)[0] != player)
    {
        moves.push_back(downRight);
    }

    return moves;
}

std::vector<Types::Coord> Logic::getVizierMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord upLeft = {coord.x - 1, coord.y - 1};
    if (upLeft.x >= 0 && upLeft.x <= Chessboard::rows &&
        upLeft.y >= 0 && upLeft.y < Chessboard::cols &&
        chessboard.getPiece(upLeft)[0] != player)
    {
        moves.push_back(upLeft);
    }

    Types::Coord upRight = {coord.x + 1, coord.y - 1};
    if (upRight.x >= 0 && upRight.x <= Chessboard::rows &&
        upRight.y >= 0 && upRight.y < Chessboard::cols &&
        chessboard.getPiece(upRight)[0] != player)
    {
        moves.push_back(upRight);
    }

    Types::Coord downLeft = {coord.x - 1, coord.y + 1};
    if (downLeft.x >= 0 && downLeft.x <= Chessboard::rows &&
        downLeft.y >= 0 && downLeft.y < Chessboard::cols - 1 &&
        chessboard.getPiece(downLeft)[0] != player)
    {
        moves.push_back(downLeft);
    }

    Types::Coord downRight = {coord.x + 1, coord.y + 1};
    if (downRight.x >= 0 && downRight.x <= Chessboard::rows &&
        downRight.y >= 0 && downRight.y < Chessboard::cols - 1 &&
        chessboard.getPiece(downRight)[0] != player)
    {
        moves.push_back(downRight);
    }

    return moves;
}

std::vector<Types::Coord> Logic::getKhanMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord up = {coord.x, coord.y - 1};
    if (up.y >= 0 && chessboard.getPiece(up)[0] != player)
    {
        moves.push_back(up);
    }

    Types::Coord right = {coord.x + 1, coord.y};
    if (right.x <= Chessboard::rows && chessboard.getPiece(right)[0] != player)
    {
        moves.push_back(right);
    }

    Types::Coord down = {coord.x, coord.y + 1};
    if (down.y < Chessboard::cols - 1 && chessboard.getPiece(down)[0] != player)
    {
        moves.push_back(down);
    }

    Types::Coord left = {coord.x - 1, coord.y};
    if (left.x >= 0 && chessboard.getPiece(left)[0] != player)
    {
        moves.push_back(left);
    }

    return moves;
}

std::vector<Types::Coord> Logic::getWarEngineMoves(Types::Coord coord, char player)
{
    auto boardState = chessboard.getBoardState();
    std::vector<Types::Coord> moves;
    char enemy = (player == 'w') ? 'b' : 'w';

    Types::Coord up = {coord.x, coord.y - 2};
    if (up.y >= 0 && chessboard.getPiece(up)[0] != player)
    {
        moves.push_back(up);
    }

    Types::Coord right = {coord.x + 2, coord.y};
    if (right.x <= Chessboard::rows && chessboard.getPiece(right)[0] != player)
    {
        moves.push_back(right);
    }

    Types::Coord down = {coord.x, coord.y + 2};
    if (down.y < Chessboard::cols - 1 && chessboard.getPiece(down)[0] != player)
    {
        moves.push_back(down);
    }

    Types::Coord left = {coord.x - 2, coord.y};
    if (left.x >= 0 && chessboard.getPiece(left)[0] != player)
    {
        moves.push_back(left);
    }

    return moves;
}

// Camel
// Mongol
// Adminstrator
// Giraffe
