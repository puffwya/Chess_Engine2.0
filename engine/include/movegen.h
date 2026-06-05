#pragma once
#include <vector>
#include "position.h"
#include "move.h"

class MoveGenerator
{
public:
    static void generateMoves(const Position& pos, std::vector<Move>& moves);
};
