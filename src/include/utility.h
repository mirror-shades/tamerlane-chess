#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include <map>
#include <string>
#include "types.h"

class Utility
{
public:
    Types::Coord calculateSquare(int x, int y);
    bool clickInBoard(const int x, const int y);
};

#endif // UTILITY_H