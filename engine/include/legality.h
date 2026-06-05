#pragma once
#include "position.h"
#include "move.h"
#include <vector>

class Legality
{
public:
    static bool isKingInCheck(const Position& pos, Color side);
    static bool isMoveLegal(const Position& pos, const Move& move);

    static void filterLegalMoves(Position& pos, std::vector<Move>& moves);
};
