#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "sliders.h"

// Bishop directions (diagonals)
static const Direction bishopDirs[4] = {
    { 1,  1},
    { 1, -1},
    {-1,  1},
    {-1, -1}
};

static inline Bitboard bishopAttacksFrom(int square, Bitboard occupancy)
{
    return slidingAttacks(square, bishopDirs, 4, occupancy);
}

void generateWhiteBishopMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard bishops = pos.whiteBishops;

    Bitboard friendly =
        pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
        pos.whiteRooks | pos.whiteQueens | pos.whiteKing;

    Bitboard enemy =
        pos.blackPawns | pos.blackKnights | pos.blackBishops |
        pos.blackRooks | pos.blackQueens | pos.blackKing;

    Bitboard occupancy = friendly | enemy;

    while (bishops)
    {
        int from = bb::popLSB(bishops);

        Bitboard attacks = bishopAttacksFrom(from, occupancy);

        Bitboard targets = attacks & ~friendly;

        while (targets)
        {
            int to = bb::popLSB(targets);

            bool isCapture = enemy & (1ULL << to);

            moves.push_back({
                (uint8_t)from,
                (uint8_t)to,
                isCapture ? CAPTURE : QUIET,
                0
            });
        }
    }
}

void generateBlackBishopMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard bishops = pos.blackBishops;

    Bitboard friendly =
        pos.blackPawns | pos.blackKnights | pos.blackBishops |
        pos.blackRooks | pos.blackQueens | pos.blackKing;

    Bitboard enemy =
        pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
        pos.whiteRooks | pos.whiteQueens | pos.whiteKing;

    Bitboard occupancy = friendly | enemy;

    while (bishops)
    {
        int from = bb::popLSB(bishops);

        Bitboard attacks = bishopAttacksFrom(from, occupancy);

        Bitboard targets = attacks & ~friendly;

        while (targets)
        {
            int to = bb::popLSB(targets);

            bool isCapture = enemy & (1ULL << to);

            moves.push_back({
                (uint8_t)from,
                (uint8_t)to,
                isCapture ? CAPTURE : QUIET,
                0
            });
        }
    }
}
