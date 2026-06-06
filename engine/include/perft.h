#pragma once

#include "position.h"
#include "move.h"
#include <vector>

class Perft
{
public:
    static long long perft(Position& pos, int depth);
    static void perftDivide(Position& pos, int depth);
};
