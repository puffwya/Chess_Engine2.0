#include "attacks.h"

Bitboard knightAttacks[64];
Bitboard kingAttacks[64];

static Bitboard knightMask(int sq)
{
    Bitboard bb = 1ULL << sq;
    Bitboard attacks = 0ULL;

    if (sq % 8 <= 5)
    {
        attacks |= (bb << 10);
        attacks |= (bb >> 6);
    }

    if (sq % 8 >= 2)
    {
        attacks |= (bb << 6);
        attacks |= (bb >> 10);
    }

    if (sq % 8 <= 6)
    {
        attacks |= (bb << 17);
        attacks |= (bb >> 15);
    }

    if (sq % 8 >= 1)
    {
        attacks |= (bb << 15);
        attacks |= (bb >> 17);
    }

    return attacks;
}

static Bitboard kingMask(int sq)
{
    Bitboard attacks = 0ULL;

    int rank = sq / 8;
    int file = sq % 8;

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int df = -1; df <= 1; df++)
        {
            if (dr == 0 && df == 0)
                continue;

            int r = rank + dr;
            int f = file + df;

            if (r >= 0 && r < 8 &&
                f >= 0 && f < 8)
            {
                attacks |= (1ULL << (r * 8 + f));
            }
        }
    }

    return attacks;
}

void initKnightAttacks()
{
    for (int i = 0; i < 64; i++)
        knightAttacks[i] = knightMask(i);
}

void initKingAttacks()
{
    for (int i = 0; i < 64; i++)
        kingAttacks[i] = kingMask(i);
}
