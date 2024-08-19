#include <iostream>
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

std::vector<Types::Coord> Logic::getCamelMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getGiraffeMoves(Types::Coord coord, char player)
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

std::vector<Types::Coord> Logic::getMoves(Types::Coord coord, std::string piece, char player)
{
    std::vector<Types::Coord> _moveList;
    switch (piece[1])
    {
    case 'p':
        _moveList = getPawnMoves(coord, player);
        break;
    case 'R':
        _moveList = getRookMoves(coord, player);
        break;
    case 'T':
        _moveList = getTaliaMoves(coord, player);
        break;
    case 'K':
        _moveList = getKhanMoves(coord, player);
        break;
    case 'E':
        _moveList = getElephantMoves(coord, player);
        break;
    case 'V':
        _moveList = getVizierMoves(coord, player);
        break;
    case 'W':
        _moveList = getWarEngineMoves(coord, player);
        break;
    case 'A':
        _moveList = getAdminMoves(coord, player);
        break;
    case 'M':
        _moveList = getMongolMoves(coord, player);
        break;
    case 'C':
        _moveList = getCamelMoves(coord, player);
        break;
    case 'G':
        _moveList = getGiraffeMoves(coord, player);
        break;
    default:
        std::cerr << "Unknown piece type: " << piece << std::endl;
        break;
    }
    return _moveList;
}

std::vector<std::pair<std::string, std::vector<Types::Coord>>> Logic::getAllMoves(char player)
{
    std::vector<std::pair<std::string, std::vector<Types::Coord>>> allMoves;
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece != "---" && piece[0] == player)
            {
                Types::Coord coord = {col, row};
                std::vector<Types::Coord> moves = getMoves(coord, piece, player);
                if (!moves.empty())
                {
                    allMoves.push_back({piece, moves});
                }
            }
        }
    }
    return allMoves;
}

std::vector<Types::Coord> Logic::filterLegalMoves(const std::vector<Types::Coord> &possibleMoves, const Types::Coord &fromCoord, const std::string &piece, char player)
{
    std::vector<Types::Coord> legalMoves;

    auto originalBoard = chessboard.getBoardState(); // Save the original state

    for (const auto &toCoord : possibleMoves)
    {
        std::string targetPiece = chessboard.getPiece(toCoord);
        // Check if the target piece is untargetable (wpx or bpx)
        if (targetPiece[2] == 'x')
        {
            continue; // Skip this move
        }
        // Move the piece
        chessboard.setCell(fromCoord, "---");
        chessboard.setCell(toCoord, piece);
        auto newBoard = chessboard.getBoardState();
        // Check if the move results in the king being in check
        if (!isKingInCheck(player, newBoard))
        {
            legalMoves.push_back(toCoord);
        }

        // Revert the move
        chessboard.setBoard(originalBoard);
    }

    // Restore the original board state just to be absolutely sure
    chessboard.setBoard(originalBoard);

    return legalMoves;
}

void Logic::promotePawns(char player)
{
    int row = (player == 'w') ? 0 : 9;

    for (int col = 0; col < Chessboard::cols; col++)
    {
        std::string piece = chessboard.getPiece({col, row});
        if (piece[1] == 'p')
        {
            std::string promotedPiece = std::string(1, player);
            switch (piece[2])
            {
            case 'R':
                promotedPiece += "Rk";
                break;
            case 'M':
                promotedPiece += "Mo";
                break;
            case 'T':
                promotedPiece += "Ta";
                break;
            case 'G':
                promotedPiece += "Gi";
                break;
            case 'V':
                promotedPiece += "Vi";
                break;
            case 'K':
                promotedPiece += "K0";
                break;
            case 'A':
                promotedPiece += "Ad";
                break;
            case 'E':
                promotedPiece += "El";
                break;
            case 'C':
                promotedPiece += "Ca";
                break;
            case 'W':
                promotedPiece += "We";
                break;
            case '0':
                promotedPiece += "px";
                break;
            case '1':
                promotedPiece += "p2";
                Types::Coord pos;
                if (player == 'w')
                {
                    pos = {5, 7};
                }
                else
                {
                    pos = {5, 2};
                }
                // figure out what happens if it's landing on a king!!!
                if (chessboard.getPiece(pos)[1] == 'K')
                {
                    chessboard.setCell({col, row}, "---");
                    std::cout << "Space occupied by king, executed!" << std::endl;
                    return;
                }
                chessboard.setCell({col, row}, "---");
                chessboard.setCell(pos, promotedPiece);
                std::cout << "Promoted " << piece << " to " << promotedPiece << std::endl;
                return;
            case '2':
                promotedPiece += "K1";
                break;
            default:
                continue; // Skip if not a valid pawn
            }
            chessboard.setCell({col, row}, promotedPiece);
            std::cout << "Promoted " << piece << " to " << promotedPiece << std::endl;
        }
    }
}

void Logic::findAndSetKingPosition(Types::Coord &kingPosition, const char &player)
{
    auto boardState = chessboard.getBoardState();
    std::string king = (player == 'w') ? "wKa" : "bKa";

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            if (boardState[row][col] == king)
            {
                kingPosition = {col, row};
                return;
            }
        }
    }
}

bool Logic::isKingInCheck(const char &player, auto boardState)
{
    // Find the king's position
    Types::Coord kingPosition;
    bool kingFound = false;

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece[0] == player && piece[1] == 'K')
            {
                kingPosition = {col, row};
                kingFound = true;
                break;
            }
        }
        if (kingFound)
            break;
    }

    if (!kingFound)
    {
        std::cout << "Error: King not found!\n";
        return false;
    }

    char enemyPlayer = (player == 'w') ? 'b' : 'w';
    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece[0] == enemyPlayer)
            {
                std::vector<Types::Coord> moves = getMoves({col, row}, piece, enemyPlayer);
                for (const auto &move : moves)
                {
                    if (move == kingPosition)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool Logic::hasLegalMoves(char player)
{
    std::vector<std::pair<std::string, std::vector<Types::Coord>>> allMoves = getAllMoves(player);

    for (const auto &pieceMoves : allMoves)
    {
        const std::string &piece = pieceMoves.first;
        const std::vector<Types::Coord> &possibleMoves = pieceMoves.second;

        Types::Coord fromCoord;
        bool pieceFound = false;
        for (int row = 0; row < Chessboard::rows && !pieceFound; ++row)
        {
            for (int col = 0; col < Chessboard::cols; ++col)
            {
                if (chessboard.getPiece({col, row}) == piece)
                {
                    fromCoord = {col, row};
                    pieceFound = true;
                    break;
                }
            }
        }

        if (!pieceFound)
            continue;

        std::vector<Types::Coord> legalMoves = filterLegalMoves(possibleMoves, fromCoord, piece, player);

        if (!legalMoves.empty())
        {
            return true;
        }
    }

    return false;
}