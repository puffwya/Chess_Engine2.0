#include "bitboard.h"
#include <iostream>

int bb::popLSB(Bitboard& b)
{
    int sq = __builtin_ctzll(b);
    b &= b - 1;
    return sq;
}

int bb::lsb(Bitboard b)
{
    return __builtin_ctzll(b);
}

void bb::print(Bitboard b)
{
    for (int r = 7; r >= 0; r--)
    {
        for (int f = 0; f < 8; f++)
        {
            int sq = r * 8 + f;
            std::cout << ((b >> sq) & 1);
        }
        std::cout << "\n";
    }
}
