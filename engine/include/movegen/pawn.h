#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhitePawnMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackPawnMoves(const Position& pos, std::vector<Move>& moves);
