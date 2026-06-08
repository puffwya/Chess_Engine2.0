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

    // Slightly more meaningful development bonus
    constexpr int KNIGHT_DEVELOPED_BONUS = 20;
    constexpr int BISHOP_DEVELOPED_BONUS = 15;

    // IMPORTANT: toned down from your current version
    constexpr int OVEREXPOSED_PENALTY = 6;

    constexpr int pawnPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };

    constexpr int knightPST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    constexpr int bishopPST[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };

    constexpr int rookPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
    };

    constexpr int queenPST[64] = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    constexpr int kingPST[64] = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };

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

                score += (white ? 1 : -1) * (
                    value + pst[tableSq] / 3
                );
            });
        };

        // -------------------------
        // MATERIAL + PST
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
        // ATTACK EXPOSURE (SOFTENED)
        // -------------------------
        auto addExposurePenalty = [&](Bitboard bb, Color side)
        {
            forBits(bb, [&](int sq)
            {
                int attacks =
                    Attack::countSquareAttacks(pos, sq,
                        side == WHITE ? BLACK : WHITE);

                if (attacks >= 2)
                {
                    int penalty = (attacks - 1) * OVEREXPOSED_PENALTY;

                    score += (side == WHITE ? -penalty : penalty);
                }
            });
        };

        addExposurePenalty(pos.whiteKnights, WHITE);
        addExposurePenalty(pos.whiteBishops, WHITE);
        addExposurePenalty(pos.whiteRooks,   WHITE);
        addExposurePenalty(pos.whiteQueens,  WHITE);

        addExposurePenalty(pos.blackKnights, BLACK);
        addExposurePenalty(pos.blackBishops, BLACK);
        addExposurePenalty(pos.blackRooks,   BLACK);
        addExposurePenalty(pos.blackQueens,  BLACK);

        // -------------------------
        // DEVELOPMENT (IMPROVED LOGIC)
        // -------------------------
        forBits(pos.whiteKnights, [&](int sq)
        {
            if (sq != 1 && sq != 6)
                score += KNIGHT_DEVELOPED_BONUS;

            // bonus for central squares
            if (sq == 26 || sq == 27 || sq == 34 || sq == 35)
                score += 10;
        });

        forBits(pos.blackKnights, [&](int sq)
        {
            if (sq != 57 && sq != 62)
                score -= KNIGHT_DEVELOPED_BONUS;

            if (sq == 26 || sq == 27 || sq == 34 || sq == 35)
                score -= 10;
        });

        forBits(pos.whiteBishops, [&](int sq)
        {
            if (sq != 2 && sq != 5)
                score += BISHOP_DEVELOPED_BONUS;
        });

        forBits(pos.blackBishops, [&](int sq)
        {
            if (sq != 58 && sq != 61)
                score -= BISHOP_DEVELOPED_BONUS;
        });

        return score;
    }
}
