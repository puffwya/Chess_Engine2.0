#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhiteBishopMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackBishopMoves(const Position& pos, std::vector<Move>& moves);
