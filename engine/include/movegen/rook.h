#pragma once

#include "position.h"
#include "move.h"
#include <vector>

void generateWhiteRookMoves(const Position& pos, std::vector<Move>& moves);
void generateBlackRookMoves(const Position& pos, std::vector<Move>& moves);
