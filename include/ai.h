#pragma once
#include "types.h"
#include "gameLogic.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <random>

class AI
{
public:
    AI(Chessboard &board) : chessboard(board) {}
    Types::Turn minMax(char player, int turn, bool alt, int depth, float alpha, float beta);
    float minMaxHelper(GameLogic gameLogic, char player, int turn, bool alt, int depth, float alpha, float beta);
    std::vector<Types::Turn> generateAllLegalMoves(char player, int turn, bool alt);
    float evaluateBoard();
    float evaluatePosition(const std::string &piece, int col, int row);
    float evaluatePawnStructure(int col, int row, bool isWhite);
    float evaluatePieceMobility(const std::string &piece, int col, int row);
    float evaluateKingSafety(int col, int row, bool isWhite);
    float evaluateCenterControl(int col, int row);
    float quiescenceSearch(char player, float alpha, float beta, int maxDepth = 3);
    std::vector<Types::Turn> generateCaptureMoves(char player);

private:
    static const std::unordered_map<char, float> pieceValues;
    Chessboard &chessboard;
    std::mt19937 rng;
};