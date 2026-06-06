#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "sliders.h"
#include "movegen/rook.h"

// rook directions (orthogonal rays)
static const Direction rookDirs[4] = {
    { 1,  0},  // right
    {-1,  0},  // left
    { 0,  1},  // up
    { 0, -1}   // down
};

static inline Bitboard rookAttacksFrom(int square, Bitboard occupancy)
{
    return slidingAttacks(square, rookDirs, 4, occupancy);
}

void generateWhiteRookMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard rooks = pos.whiteRooks;

    Bitboard friendly = pos.whitePieces();
    Bitboard enemy = pos.blackPieces();

    Bitboard occupancy = pos.occupancy();

    while (rooks)
    {
        int from = bb::popLSB(rooks);

        Bitboard attacks = rookAttacksFrom(from, occupancy);
        Bitboard targets = attacks & ~friendly;

        while (targets)
        {
            int to = bb::popLSB(targets);

            bool isCapture = enemy & (1ULL << to);

            moves.push_back({
                (uint8_t)from,
                (uint8_t)to,
                isCapture ? CAPTURE : QUIET,
                NONE
            });
        }
    }
}

void generateBlackRookMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard rooks = pos.blackRooks;

    Bitboard friendly = pos.blackPieces();
    Bitboard enemy = pos.whitePieces();

    Bitboard occupancy = pos.occupancy();

    while (rooks)
    {
        int from = bb::popLSB(rooks);

        Bitboard attacks = rookAttacksFrom(from, occupancy);
        Bitboard targets = attacks & ~friendly;

        while (targets)
        {
            int to = bb::popLSB(targets);

            bool isCapture = enemy & (1ULL << to);

            moves.push_back({
                (uint8_t)from,
                (uint8_t)to,
                isCapture ? CAPTURE : QUIET,
                NONE
            });
        }
    }
}
