// Copyright 2024. mirror-shades. GPL-2.0 License.
#include <iostream>
#include <vector>
#include <string>
#include "gameLogic.h"
#include "pieceLogic.h"
#include "globals.h"

PieceLogic pieceLogic;

// functionality
std::vector<Types::Coord> GameLogic::getMoves(Types::Coord coord,
                                              Types::Piece piece,
                                              char player,
                                              bool alt)
{
    std::vector<Types::Coord> moveList;

    if (piece.piece() == 'R')
    {
        moveList = pieceLogic.getRookMoves(coord, player);
    }
    else if (piece.piece() == 'T')
    {
        moveList = pieceLogic.getTaliaMoves(coord, player);
    }
    else if (piece.piece() == 'K')
    {
        moveList = pieceLogic.getKhanMoves(coord, player);
    }
    else if (piece.piece() == 'M')
    {
        moveList = pieceLogic.getMongolMoves(coord, player);
    }
    else if (piece.piece() == 'C')
    {
        moveList = pieceLogic.getCamelMoves(coord, player);
    }
    else if (piece.piece() == 'G')
    {
        moveList = pieceLogic.getGiraffeMoves(coord, player);
    }
    else if (piece.piece() == 'p')
    {
        moveList = alt ? pieceLogic.getAltPawnMoves(coord, player)
                       : pieceLogic.getPawnMoves(coord, player);
    }
    else if (piece.piece() == 'E')
    {
        moveList = alt ? pieceLogic.getAltElephantMoves(coord, player)
                       : pieceLogic.getElephantMoves(coord, player);
    }
    else if (piece.piece() == 'W')
    {
        moveList = alt ? pieceLogic.getAltWarEngineMoves(coord, player)
                       : pieceLogic.getWarEngineMoves(coord, player);
    }
    else if (piece.piece() == 'V')
    {
        moveList = alt ? pieceLogic.getAltVizierMoves(coord, player)
                       : pieceLogic.getVizierMoves(coord, player);
    }
    else if (piece.piece() == 'A')
    {
        moveList = alt ? pieceLogic.getAltAdminMoves(coord, player)
                       : pieceLogic.getAdminMoves(coord, player);
    }
    else
    {
        std::cerr << "Unknown piece type: " << piece.toString() << std::endl;
    }

    return moveList;
}

// returns a vector of pairs of strings and vectors of coords
// {{"piece", {coords}}, {"piece", {coords}}, ...}
std::vector<std::pair<Types::Piece, std::vector<Types::Coord>>>
GameLogic::getAllMoves(char player,
                       bool alt)
{
    std::vector<std::pair<Types::Piece, std::vector<Types::Coord>>> allMoves;
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Piece piece = boardState.board[row][col];
            if (piece != "---" && piece.color() == player)
            {
                Types::Coord coord = {col, row};
                std::vector<Types::Coord> moves = getMoves(coord,
                                                           piece,
                                                           player,
                                                           alt);
                if (!moves.empty())
                {
                    allMoves.push_back({piece, moves});
                }
            }
        }
    }
    return allMoves;
}

std::vector<Types::Coord>
GameLogic::filterLegalMoves(const std::vector<Types::Coord> &possibleMoves,
                            const Types::Coord &fromCoord,
                            const Types::Piece &piece,
                            char player,
                            bool alt)
{
    std::vector<Types::Coord> legalMoves;

    // Backup the original state
    const Types::Board originalBoard = chessboard.getBoardState();

    for (const auto &toCoord : possibleMoves)
    {
        Types::Piece targetPiece = chessboard.getPiece(toCoord);
        // Check if the target piece is untargetable (wpx or bpx)
        if (targetPiece.variant() == 'x')
        {
            // Skip this move, pawns as untargetable
            // in this stage of promotion
            continue;
        }
        // Move the piece
        chessboard.setCell(fromCoord, "---");
        chessboard.setCell(toCoord, piece);
        Types::Board newBoard = chessboard.getBoardState();
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
        Types::Piece piece = chessboard.getPiece({col, row});
        if (piece.piece() != 'p')
            continue;

        std::string promotionType;

        switch (piece.variant())
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
            Types::Piece targetPiece = chessboard.getPiece(pos);
            if (targetPiece.piece() == 'K')
            {
                chessboard.setCell({col, row}, "---");
                std::cout << "Space occupied by king, pawn executed!"
                          << std::endl;
            }
            else
            {
                chessboard.setCell({col, row}, "---");
                chessboard.setCell(pos, Types::Piece(player + std::string(promotionType)));
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
            std::cout << "Invalid pawn type: "
                      << piece.toString() << " at "
                      << col << "," << row << std::endl;
            continue;
        }

        chessboard.setCell({col, row}, Types::Piece(player + promotionType));
        std::cout << "Promoted "
                  << piece.toString() << " to "
                  << player + promotionType << " at "
                  << col << "," << row << std::endl;
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
            Types::Piece piece1 = chessboard.getPiece({col, row});
            Types::Piece piece2 = chessboard.getPiece({col + 2, row});

            if (piece1.color() == enemy && piece2.color() == enemy)
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

                Types::Piece targetPiece =
                    chessboard.getPiece({forkCol, forkRow});
                if (targetPiece == "---" ||
                    (targetPiece.color() == enemy && targetPiece.piece() != 'K'))
                {
                    // Move the pawnX to the fork position
                    chessboard.setCell(pawnXPos, "---");
                    chessboard.setCell({forkCol, forkRow}, Types::Piece(player + "p1"));
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
            Types::Piece piece = boardState.board[row][col];
            if (piece.toString() == player + "px")
            {
                return {col, row};
            }
        }
    }
    return {-1, -1};
}

void GameLogic::findAndSetKingPosition(Types::Coord &kingPosition,
                                       const char &player)
{
    auto boardState = chessboard.getBoardState();
    Types::Piece king = (player == 'w') ? "wKa" : "bKa";

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            if (boardState.board[row][col] == king)
            {
                kingPosition = {col, row};
                return;
            }
        }
    }
}

bool GameLogic::isKingInCheck(const char &player,
                              const Types::Board &boardState,
                              bool alt)
{
    // Find the king's position
    Types::Coord kingPosition;
    bool kingFound = false;

    for (int row = 0; row < Chessboard::rows; ++row)
    {
        for (int col = 0; col < Chessboard::cols; ++col)
        {
            Types::Piece piece = boardState.board[row][col];
            if (piece.color() == player && piece.piece() == 'K')
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
            Types::Piece piece = boardState.board[row][col];
            if (piece.color() == enemyPlayer)
            {
                std::vector<Types::Coord> moves = getMoves({col, row},
                                                           piece,
                                                           enemyPlayer,
                                                           alt);
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
    std::vector<std::pair<Types::Piece, std::vector<Types::Coord>>>
        allMoves = getAllMoves(player, alt);

    for (const auto &pieceMoves : allMoves)
    {
        const Types::Piece &piece = pieceMoves.first;
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

        std::vector<Types::Coord> legalMoves = filterLegalMoves(possibleMoves,
                                                                fromCoord,
                                                                piece,
                                                                player,
                                                                alt);

        if (!legalMoves.empty())
        {
            return true;
        }
    }

    return false;
}

bool GameLogic::canDraw(char player)
{
    if (player == 'w')
    {
        if (chessboard.getPiece({0, 0}) == "wKa" ||
            chessboard.getPiece({0, 1}) == "wKa" ||
            chessboard.getPiece({0, 2}) == "wKa")
        {
            return true;
        }
    }
    else
    {
        if (chessboard.getPiece({10, 7}) == "bKa" ||
            chessboard.getPiece({10, 8}) == "bKa" ||
            chessboard.getPiece({10, 9}) == "bKa")
        {
            return true;
        }
    }
    return false;
}
