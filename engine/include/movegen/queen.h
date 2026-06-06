#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhiteQueenMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackQueenMoves(const Position& pos, std::vector<Move>& moves);
