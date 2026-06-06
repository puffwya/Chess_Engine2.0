#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhiteKnightMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackKnightMoves(const Position& pos, std::vector<Move>& moves);
