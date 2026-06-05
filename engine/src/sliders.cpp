#include "sliders.h"

static inline bool onBoard(int f, int r)
{
    return f >= 0 && f < 8 && r >= 0 && r < 8;
}

Bitboard slidingAttacks(int square, const Direction* dirs, int dirCount, Bitboard occupancy)
{
    Bitboard attacks = 0ULL;

    int f = square % 8;
    int r = square / 8;

    for (int d = 0; d < dirCount; d++)
    {
        int nf = f + dirs[d].df;
        int nr = r + dirs[d].dr;

        while (onBoard(nf, nr))
        {
            int to = nr * 8 + nf;
            Bitboard toBB = 1ULL << to;

            attacks |= toBB;

            if (occupancy & toBB)
                break;

            nf += dirs[d].df;
            nr += dirs[d].dr;
        }
    }

    return attacks;
}
