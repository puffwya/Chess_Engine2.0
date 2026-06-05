#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "sliders.h"

// combine rook + bishop directions

static const Direction queenDirs[8] = {
    { 1,  0},  // right
    {-1,  0},  // left
    { 0,  1},  // up
    { 0, -1},  // down

    { 1,  1},  // diag up-right
    { 1, -1},  // diag down-right
    {-1,  1},  // diag up-left
    {-1, -1}   // diag down-left
};

static inline Bitboard queenAttacksFrom(int square, Bitboard occupancy)
{
    return slidingAttacks(square, queenDirs, 8, occupancy);
}

void generateWhiteQueenMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard queens = pos.whiteQueens;

    Bitboard friendly = pos.whitePieces();
    Bitboard enemy = pos.blackPieces();

    Bitboard occupancy = pos.occupancy();

    while (queens)
    {
        int from = bb::popLSB(queens);

        Bitboard attacks = queenAttacksFrom(from, occupancy);
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

void generateBlackQueenMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard queens = pos.blackQueens;

    Bitboard friendly = pos.blackPieces();
    Bitboard enemy = pos.whitePieces();

    Bitboard occupancy = pos.occupancy();

    while (queens)
    {
        int from = bb::popLSB(queens);

        Bitboard attacks = queenAttacksFrom(from, occupancy);
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
