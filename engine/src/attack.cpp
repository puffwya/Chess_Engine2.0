#include "attack.h"
#include "attacks.h"
#include "sliders.h"

bool Attack::isSquareAttacked(
    const Position& pos,
    int square,
    Color attackerSide)
{
    Bitboard target = 1ULL << square;
    Bitboard occ = pos.occupancy();

    //
    // Pawn attacks
    //

    if (attackerSide == WHITE)
    {
        Bitboard pawns = pos.whitePawns;

        Bitboard attacks =
            ((pawns << 7) & ~FILE_H) |
            ((pawns << 9) & ~FILE_A);

        if (attacks & target)
            return true;
    }
    else
    {
        Bitboard pawns = pos.blackPawns;

        Bitboard attacks =
            ((pawns >> 7) & ~FILE_A) |
            ((pawns >> 9) & ~FILE_H);

        if (attacks & target)
            return true;
    }

    //
    // Knights
    //

    Bitboard knights =
        attackerSide == WHITE
            ? pos.whiteKnights
            : pos.blackKnights;

    while (knights)
    {
        int sq = bb::popLSB(knights);

        if (knightAttacks[sq] & target)
            return true;
    }

    //
    // Kings
    //

    Bitboard king =
        attackerSide == WHITE
            ? pos.whiteKing
            : pos.blackKing;

    if (king)
    {
        int sq = bb::lsb(king);

        if (kingAttacks[sq] & target)
            return true;
    }

    //
    // Bishops / Queens
    //

    static const Direction bishopDirs[4] =
    {
        { 1, 1 },
        { 1,-1 },
        {-1, 1 },
        {-1,-1 }
    };

    Bitboard bishops =
        attackerSide == WHITE
            ? (pos.whiteBishops | pos.whiteQueens)
            : (pos.blackBishops | pos.blackQueens);

    Bitboard temp = bishops;

    while (temp)
    {
        int sq = bb::popLSB(temp);

        if (slidingAttacks(
                sq,
                bishopDirs,
                4,
                occ) & target)
        {
            return true;
        }
    }

    //
    // Rooks / Queens
    //

    static const Direction rookDirs[4] =
    {
        { 1, 0 },
        {-1, 0 },
        { 0, 1 },
        { 0,-1 }
    };

    Bitboard rooks =
        attackerSide == WHITE
            ? (pos.whiteRooks | pos.whiteQueens)
            : (pos.blackRooks | pos.blackQueens);

    temp = rooks;

    while (temp)
    {
        int sq = bb::popLSB(temp);

        if (slidingAttacks(
                sq,
                rookDirs,
                4,
                occ) & target)
        {
            return true;
        }
    }

    return false;
}

bool Attack::isKingInCheck(const Position& pos, Color side)
{
    int kingSquare;

    if (side == WHITE)
        kingSquare = bb::lsb(pos.whiteKing);
    else
        kingSquare = bb::lsb(pos.blackKing);

    // if opponent attacks king square → check
    return isSquareAttacked(
        pos,
        kingSquare,
        side == WHITE ? BLACK : WHITE
    );
}
