#include "evaluate.h"
#include <cstdint>
#include <bit>

namespace
{
    constexpr int PAWN_VALUE   = 100;
    constexpr int KNIGHT_VALUE = 320;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE   = 500;
    constexpr int QUEEN_VALUE  = 900;

    // --- Piece Square Tables (same idea as your old engine) ---

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
}

namespace Evaluate
{
    int evaluate(const Position& pos)
    {
        int score = 0;

        auto evalPieces = [&](Bitboard bb, int pieceValue, const int* pst, bool white)
        {
            while (bb)
            {
                int sq = __builtin_ctzll(bb);
                bb &= bb - 1;

                int tableSq = white ? sq : mirror(sq);

                score += (white ? 1 : -1) * (pieceValue + pst[tableSq]);
            }
        };

        evalPieces(pos.whitePawns,   PAWN_VALUE,   pawnPST,   true);
        evalPieces(pos.blackPawns,   PAWN_VALUE,   pawnPST,   false);

        evalPieces(pos.whiteKnights, KNIGHT_VALUE, knightPST, true);
        evalPieces(pos.blackKnights, KNIGHT_VALUE, knightPST, false);

        evalPieces(pos.whiteBishops, BISHOP_VALUE, bishopPST, true);
        evalPieces(pos.blackBishops, BISHOP_VALUE, bishopPST, false);

        evalPieces(pos.whiteRooks,   ROOK_VALUE,   rookPST,   true);
        evalPieces(pos.blackRooks,   ROOK_VALUE,   rookPST,   false);

        evalPieces(pos.whiteQueens,  QUEEN_VALUE,  queenPST,  true);
        evalPieces(pos.blackQueens,  QUEEN_VALUE,  queenPST,  false);

        evalPieces(pos.whiteKing,    20000,        kingPST,   true);
        evalPieces(pos.blackKing,    20000,        kingPST,   false);

        return score;
    }
}
