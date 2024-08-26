#pragma once
#include "types.h"

class AI
{
public:
    Types::Turn getAIMove(const char player, int turn, bool alt);
};