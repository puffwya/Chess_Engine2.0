#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhiteKingMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackKingMoves(const Position& pos, std::vector<Move>& moves);
