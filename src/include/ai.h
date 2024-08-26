#pragma once
#include "types.h"
#include "gameLogic.h"

class AI
{
public:
    Types::Turn getRandomAIMove(const char player, int turn, bool alt);
    Types::Turn getGreedyAIMove(const char player, int turn, bool alt);
    Types::Turn getMinMaxAIMove(const char player, int turn, bool alt, int depth);
    float minMax(GameLogic &gameLogic, char player, int turn, bool alt, int depth, float alpha, float beta);
    float evaluateBoard();
};