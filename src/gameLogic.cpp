#include <iostream>
#include "include/gameLogic.h"
#include "include/pieceLogic.h"
#include "include/globals.h"

PieceLogic pieceLogic;

// functionality
std::vector<Types::Coord> GameLogic::getMoves(Types::Coord coord, std::string piece, char player, bool alt)
{
    std::vector<Types::Coord> _moveList;

    if (piece[1] == 'R')
    {
        _moveList = pieceLogic.getRookMoves(coord, player);
    }
    else if (piece[1] == 'T')
    {
        _moveList = pieceLogic.PieceLogic::getTaliaMoves(coord, player);
    }
    else if (piece[1] == 'K')
    {
        _moveList = pieceLogic.PieceLogic::getKhanMoves(coord, player);
    }
    else if (piece[1] == 'M')
    {
        _moveList = pieceLogic.PieceLogic::getMongolMoves(coord, player);
    }
    else if (piece[1] == 'C')
    {
        _moveList = pieceLogic.PieceLogic::getCamelMoves(coord, player);
    }
    else if (piece[1] == 'G')
    {
        _moveList = pieceLogic.PieceLogic::getGiraffeMoves(coord, player);
    }
    else if (piece[1] == 'p')
    {
        _moveList = alt ? pieceLogic.PieceLogic::getAltPawnMoves(coord, player) : pieceLogic.PieceLogic::getPawnMoves(coord, player);
    }
    else if (piece[1] == 'E')
    {
        _moveList = alt ? pieceLogic.PieceLogic::getAltElephantMoves(coord, player) : pieceLogic.PieceLogic::getElephantMoves(coord, player);
    }
    else if (piece[1] == 'W')
    {
        _moveList = alt ? pieceLogic.PieceLogic::getAltWarEngineMoves(coord, player) : pieceLogic.PieceLogic::getWarEngineMoves(coord, player);
    }
    else if (piece[1] == 'V')
    {
        _moveList = alt ? pieceLogic.PieceLogic::getAltVizierMoves(coord, player) : pieceLogic.PieceLogic::getVizierMoves(coord, player);
    }
    else if (piece[1] == 'A')
    {
        _moveList = alt ? pieceLogic.PieceLogic::getAltAdminMoves(coord, player) : pieceLogic.PieceLogic::getAdminMoves(coord, player);
    }
    else
    {
        std::cerr << "Unknown piece type: " << piece << std::endl;
    }

    return _moveList;
}

std::vector<std::pair<std::string, std::vector<Types::Coord>>> GameLogic::getAllMoves(char player, bool alt)
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
                std::vector<Types::Coord> moves = getMoves(coord, piece, player, alt);
                if (!moves.empty())
                {
                    allMoves.push_back({piece, moves});
                }
            }
        }
    }
    return allMoves;
}

std::vector<Types::Coord> GameLogic::filterLegalMoves(const std::vector<Types::Coord> &possibleMoves, const Types::Coord &fromCoord, const std::string &piece, char player, bool alt)
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
        if (!isKingInCheck(player, newBoard, alt))
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

void GameLogic::promotePawns(char player)
{
    int row = (player == 'w') ? 0 : 9;
    char enemy = (player == 'w') ? 'b' : 'w';

    for (int col = 0; col < Chessboard::cols; col++)
    {
        std::string piece = chessboard.getPiece({col, row});
        if (piece[1] != 'p')
            continue;

        std::string promotedPiece = std::string(1, player);
        std::string promotionType;

        switch (piece[2])
        {
        case 'R':
            promotionType = "Rk";
            break;
        case 'M':
            promotionType = "Mo";
            break;
        case 'T':
            promotionType = "Ta";
            break;
        case 'G':
            promotionType = "Gi";
            break;
        case 'V':
            promotionType = "Vi";
            break;
        case 'K':
            promotionType = "K0";
            break;
        case 'A':
            promotionType = "Ad";
            break;
        case 'E':
            promotionType = "El";
            break;
        case 'C':
            promotionType = "Ca";
            break;
        case 'W':
            promotionType = "We";
            break;
        case '0':
            promotionType = "px";
            break;
        case '1':
        {
            promotionType = "p2";
            Types::Coord pos = {5, player == 'w' ? 7 : 2};
            std::string targetPiece = chessboard.getPiece(pos);
            if (targetPiece[1] == 'K')
            {
                chessboard.setCell({col, row}, "---");
                std::cout << "Space occupied by king, pawn executed!" << std::endl;
            }
            else
            {
                chessboard.setCell({col, row}, "---");
                chessboard.setCell(pos, player + promotionType);
                std::cout << "Promoted " << piece << " to " << player + promotionType << " at " << pos.x << "," << pos.y << std::endl;
            }
            continue;
        }
        case '2':
            promotionType = "K1";
            break;
        case 'x':
            checkPawnForks(enemy);
            continue;
        default:
            std::cout << "Invalid pawn type: " << piece << " at " << col << "," << row << std::endl;
            continue;
        }

        promotedPiece += promotionType;
        chessboard.setCell({col, row}, promotedPiece);
        std::cout << "Promoted " << piece << " to " << promotedPiece << " at " << col << "," << row << std::endl;
    }
}

void GameLogic::checkPawnForks(char player)
{
    char enemy = (player == 'w') ? 'b' : 'w';
    Types::Coord pawnXPos = findPawnX(player);

    if (pawnXPos.x == -1 || pawnXPos.y == -1)
    {
        std::cout << "No pawnX found for player " << player << std::endl;
        return;
    }

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece1 = chessboard.getPiece({col, row});
            std::string piece2 = chessboard.getPiece({col + 2, row});

            if (piece1[0] == enemy && piece2[0] == enemy)
            {
                int forkCol = col;
                int forkRow = row + 1;

                // Check if the fork position is valid
                if (player == 'w' && forkCol < Chessboard::cols - 1)
                {
                    forkCol++;
                }
                else if (player == 'b' && forkCol > 0)
                {
                    forkCol--;
                }

                std::string targetPiece = chessboard.getPiece({forkCol, forkRow});
                if (targetPiece == "---" || (targetPiece[0] == enemy && targetPiece[1] != 'K'))
                {
                    // Move the pawnX to the fork position
                    chessboard.setCell(pawnXPos, "---");
                    chessboard.setCell({forkCol, forkRow}, std::string(1, player) + "p1");
                    std::cout << "Pawn fork placed at " << forkCol << ", " << forkRow << std::endl;
                    return;
                }
            }
        }
    }
    std::cout << "No valid pawn fork found for player " << player << std::endl;
}

Types::Coord GameLogic::findPawnX(char player)
{
    auto boardState = chessboard.getBoardState();
    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece[0] == player && piece[1] == 'p' && piece[2] == 'x')
            {
                return {col, row};
            }
        }
    }
    return {-1, -1};
}

void GameLogic::findAndSetKingPosition(Types::Coord &kingPosition, const char &player)
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

bool GameLogic::isKingInCheck(const char &player, auto boardState, bool alt)
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
                std::vector<Types::Coord> moves = getMoves({col, row}, piece, enemyPlayer, alt);
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

bool GameLogic::hasLegalMoves(char player, bool alt)
{
    std::vector<std::pair<std::string, std::vector<Types::Coord>>> allMoves = getAllMoves(player, alt);

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

        std::vector<Types::Coord> legalMoves = filterLegalMoves(possibleMoves, fromCoord, piece, player, alt);

        if (!legalMoves.empty())
        {
            return true;
        }
    }

    return false;
}

bool GameLogic::canDraw(char player)
{
    auto boardState = chessboard.getBoardState();
    std::cout << "Checking for draw" << std::endl;
    std::cout << "Player: " << player << std::endl;
    std::cout << "Board state: " << boardState[0][0] << std::endl;
    std::cout << "Board state: " << boardState[1][0] << std::endl;
    std::cout << "Board state: " << boardState[2][0] << std::endl;
    std::cout << "Board state: " << boardState[7][10] << std::endl;
    std::cout << "Board state: " << boardState[8][10] << std::endl;
    std::cout << "Board state: " << boardState[9][10] << std::endl;
    if (player == 'w')
    {
        if (boardState[0][0] == "wKa" || boardState[1][0] == "wKa" || boardState[2][0] == "wKa")
        {
            return true;
        }
    }
    else
    {
        if (boardState[8][9] == "bKa" || boardState[9][9] == "bKa" || boardState[10][9] == "bKa")
        {
            return true;
        }
    }
    return false;
}