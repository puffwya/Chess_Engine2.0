#pragma once

#include "position.h"

class Attack
{
public:
    static bool isSquareAttacked(const Position& pos, int sq, Color attacker);

    static bool isKingInCheck(const Position& pos, Color side);

    static int countSquareAttacks(const Position& pos, int sq, Color attacker);
};
