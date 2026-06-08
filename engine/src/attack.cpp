#include "attack.h"
#include "attacks.h"
#include "sliders.h"

bool Attack::isSquareAttacked(const Position& pos, int square, Color attackerSide)
{
    Bitboard target = 1ULL << square;
    Bitboard occ = pos.occupancy();

    // ---------------------
    // Pawns
    // ---------------------
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

    // ---------------------
    // Knights
    // ---------------------
    Bitboard knights = (attackerSide == WHITE)
        ? pos.whiteKnights
        : pos.blackKnights;

    while (knights)
    {
        int sq = bb::popLSB(knights);
        if (knightAttacks[sq] & target)
            return true;
    }

    // ---------------------
    // King
    // ---------------------
    Bitboard king = (attackerSide == WHITE)
        ? pos.whiteKing
        : pos.blackKing;

    if (king)
    {
        int sq = bb::lsb(king);
        if (kingAttacks[sq] & target)
            return true;
    }

    // ---------------------
    // Bishops + Queens
    // ---------------------
    static const Direction bishopDirs[4] = {
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    Bitboard bq = (attackerSide == WHITE)
        ? (pos.whiteBishops | pos.whiteQueens)
        : (pos.blackBishops | pos.blackQueens);

    Bitboard temp = bq;

    while (temp)
    {
        int sq = bb::popLSB(temp);

        if (slidingAttacks(sq, bishopDirs, 4, occ) & target)
            return true;
    }

    // ---------------------
    // Rooks + Queens
    // ---------------------
    static const Direction rookDirs[4] = {
        {1,0},{-1,0},{0,1},{0,-1}
    };

    Bitboard rq = (attackerSide == WHITE)
        ? (pos.whiteRooks | pos.whiteQueens)
        : (pos.blackRooks | pos.blackQueens);

    temp = rq;

    while (temp)
    {
        int sq = bb::popLSB(temp);

        if (slidingAttacks(sq, rookDirs, 4, occ) & target)
            return true;
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

int Attack::countSquareAttacks(const Position& pos, int sq, Color attacker)
{
    int count = 0;
    Bitboard targetBB = 1ULL << sq;

    if (attacker == WHITE)
    {
        // --- KNIGHTS ---
        Bitboard knights = pos.whiteKnights;
        while (knights)
        {
            int from = __builtin_ctzll(knights);
            knights &= knights - 1;

            if (knightAttacks[from] & targetBB)
                count++;
        }

        // --- KING ---
        if (kingAttacks[__builtin_ctzll(pos.whiteKing)] & targetBB)
            count++;

        // --- PAWNS (WHITE attacks up-left / up-right) ---
        Bitboard pawns = pos.whitePawns;
        while (pawns)
        {
            int from = __builtin_ctzll(pawns);
            pawns &= pawns - 1;

            Bitboard bb = 1ULL << from;

            if ((bb << 7) & targetBB && from % 8 != 0) count++;
            if ((bb << 9) & targetBB && from % 8 != 7) count++;
        }
    }
    else
    {
        // --- KNIGHTS ---
        Bitboard knights = pos.blackKnights;
        while (knights)
        {
            int from = __builtin_ctzll(knights);
            knights &= knights - 1;

            if (knightAttacks[from] & targetBB)
                count++;
        }

        // --- KING ---
        if (kingAttacks[__builtin_ctzll(pos.blackKing)] & targetBB)
            count++;

        // --- PAWNS (BLACK attacks down-left / down-right) ---
        Bitboard pawns = pos.blackPawns;
        while (pawns)
        {
            int from = __builtin_ctzll(pawns);
            pawns &= pawns - 1;

            Bitboard bb = 1ULL << from;

            if ((bb >> 7) & targetBB && from % 8 != 7) count++;
            if ((bb >> 9) & targetBB && from % 8 != 0) count++;
        }
    }

    return count;
}
