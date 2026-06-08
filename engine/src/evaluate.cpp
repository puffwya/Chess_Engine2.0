#include "evaluate.h"
#include <cstdint>
#include <bit>
#include "../include/attack.h"

namespace
{
    constexpr int PAWN_VALUE   = 100;
    constexpr int KNIGHT_VALUE = 320;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE   = 500;
    constexpr int QUEEN_VALUE  = 900;

    constexpr int KNIGHT_DEVELOPED_BONUS = 18;
    constexpr int BISHOP_DEVELOPED_BONUS = 12;

    constexpr int OVEREXPOSED_PENALTY = 5;

    // NEW: safety tuning
    constexpr int SAFETY_FACTOR = 12;

    constexpr int pawnPST[64]   = { /* unchanged */ };
    constexpr int knightPST[64] = { /* unchanged */ };
    constexpr int bishopPST[64] = { /* unchanged */ };
    constexpr int rookPST[64]   = { /* unchanged */ };
    constexpr int queenPST[64]  = { /* unchanged */ };
    constexpr int kingPST[64]   = { /* unchanged */ };

    inline int mirror(int sq)
    {
        return (7 - (sq / 8)) * 8 + (sq % 8);
    }

    template<typename Fn>
    inline void forBits(Bitboard bb, Fn fn)
    {
        while (bb)
        {
            int sq = __builtin_ctzll(bb);
            bb &= bb - 1;
            fn(sq);
        }
    }

    // ============================================================
    // NEW: 1-ply safety evaluation (cheap "is this square unsafe?")
    // ============================================================
    inline int safetyPenalty(const Position& pos, int sq, Color side)
    {
        Color enemy = (side == WHITE ? BLACK : WHITE);

        int attacks = Attack::countSquareAttacks(pos, sq, enemy);

        if (attacks == 0)
            return 0;

        // small pressure = small penalty, heavy pressure = bigger penalty
        return attacks * SAFETY_FACTOR;
    }
}

namespace Evaluate
{
    int evaluate(const Position& pos)
    {
        int score = 0;

        auto addPiece = [&](Bitboard bb, int value, const int* pst, bool white)
        {
            forBits(bb, [&](int sq)
            {
                int tableSq = white ? sq : mirror(sq);

                int base = value + pst[tableSq] / 4;

                // NEW: safety penalty per piece
                int safe = safetyPenalty(pos, sq, white ? WHITE : BLACK);

                score += (white ? 1 : -1) * (base - safe);
            });
        };

        // -------------------------
        // MATERIAL + PST + SAFETY
        // -------------------------
        addPiece(pos.whitePawns,   PAWN_VALUE,   pawnPST,   true);
        addPiece(pos.blackPawns,   PAWN_VALUE,   pawnPST,   false);

        addPiece(pos.whiteKnights, KNIGHT_VALUE, knightPST, true);
        addPiece(pos.blackKnights, KNIGHT_VALUE, knightPST, false);

        addPiece(pos.whiteBishops, BISHOP_VALUE, bishopPST, true);
        addPiece(pos.blackBishops, BISHOP_VALUE, bishopPST, false);

        addPiece(pos.whiteRooks,   ROOK_VALUE,   rookPST,   true);
        addPiece(pos.blackRooks,   ROOK_VALUE,   rookPST,   false);

        addPiece(pos.whiteQueens,  QUEEN_VALUE,  queenPST,  true);
        addPiece(pos.blackQueens,  QUEEN_VALUE,  queenPST,  false);

        addPiece(pos.whiteKing,    20000,        kingPST,   true);
        addPiece(pos.blackKing,    20000,        kingPST,   false);

        // -------------------------
        // ATTACK PRESSURE (existing)
        // -------------------------
        auto pressure = [&](Bitboard bb, Color side)
        {
            forBits(bb, [&](int sq)
            {
                int att =
                    Attack::countSquareAttacks(pos, sq,
                        side == WHITE ? BLACK : WHITE);

                if (att > 1)
                    score += (side == WHITE ? -1 : 1) * (att - 1) * OVEREXPOSED_PENALTY;
            });
        };

        pressure(pos.whiteKnights, WHITE);
        pressure(pos.whiteBishops, WHITE);
        pressure(pos.whiteRooks,   WHITE);
        pressure(pos.whiteQueens,  WHITE);

        pressure(pos.blackKnights, BLACK);
        pressure(pos.blackBishops, BLACK);
        pressure(pos.blackRooks,   BLACK);
        pressure(pos.blackQueens,  BLACK);

        // -------------------------
        // DEVELOPMENT + OPENING BEHAVIOR FIX
        // -------------------------

        forBits(pos.whiteKnights, [&](int sq)
        {
            if (sq != 1 && sq != 6)
                score += KNIGHT_DEVELOPED_BONUS;

            // discourage early central knight jumps before development
            if (sq == 18 || sq == 21)
                score -= 8;

            if (sq == 26 || sq == 27 || sq == 34 || sq == 35)
                score -= 20;
        });

        forBits(pos.blackKnights, [&](int sq)
        {
            if (sq != 57 && sq != 62)
                score -= KNIGHT_DEVELOPED_BONUS;

            if (sq == 42 || sq == 45)
                score += 8;

            if (sq == 26 || sq == 27 || sq == 34 || sq == 35)
                score += 20;
        });

        // -------------------------
        // OPENING DISCIPLINE
        // -------------------------

        int totalPieces =
            __builtin_popcountll(pos.whitePieces()) +
            __builtin_popcountll(pos.blackPieces());

        bool opening = (totalPieces > 24);

        if (opening)
        {
            // Penalize moving SAME knight too early into aggressive squares
            // BUT DO NOT bias board direction (important fix)

            auto knightPenalty = [&](Bitboard knights, Color side)
            {
                forBits(knights, [&](int sq)
                {
                    int file = sq % 8;
                    int rank = sq / 8;

                    bool isEdge = (file == 0 || file == 7);

                    // mild penalty for early rim activity (not directional)
                    if (isEdge)
                        score += (side == WHITE ? -3 : 3);

                    // mild encouragement to stay closer to center early
                    int centerDist =
                        abs(3 - file) + abs(3 - rank);

                    if (centerDist <= 2)
                        score += (side == WHITE ? 2 : -2);
                });
            };

            knightPenalty(pos.whiteKnights, WHITE);
            knightPenalty(pos.blackKnights, BLACK);
        }

        return score;
    }
}
