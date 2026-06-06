#pragma once

#include "position.h"

class Attack
{
public:
    static bool isSquareAttacked(
        const Position& pos,
        int square,
        Color attackerSide);
};
