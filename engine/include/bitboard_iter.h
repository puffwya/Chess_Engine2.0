#pragma once
#include "bitboard.h"

namespace bb
{
    inline int popLSB(Bitboard& b)
    {
        int sq = __builtin_ctzll(b);
        b &= b - 1;
        return sq;
    }
}
