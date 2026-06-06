#include "bitboard.h"
#include <iostream>

namespace bb
{
    int lsb(Bitboard b)
    {
        return __builtin_ctzll(b);
    }

    int popLSB(Bitboard& b)
    {
        int sq = __builtin_ctzll(b);
        b &= (b - 1);
        return sq;
    }

    void print(Bitboard b)
    {
        for (int r = 7; r >= 0; r--)
        {
            for (int f = 0; f < 8; f++)
            {
                std::cout << ((b >> (r * 8 + f)) & 1);
            }
            std::cout << "\n";
        }
    }
}
