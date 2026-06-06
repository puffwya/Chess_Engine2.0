#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "movegen/pawn.h"

void generateWhitePawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.whitePawns;

    Bitboard empty = ~(pos.whitePieces() | pos.blackPieces());

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        // -------------------------
        // single push
        // -------------------------
        Bitboard one = (fromBB << 8) & empty;

        if (one)
        {
            int to = bb::lsb(one);
            moves.push_back({(uint8_t)from, (uint8_t)to, QUIET, 0});

            // -------------------------
            // double push (ONLY from rank 2)
            // rank 2 = squares 8–15
            // -------------------------
            if (from / 8 == 1)
            {
                Bitboard two = (fromBB << 16) & empty;

                // IMPORTANT: also ensure intermediate square is empty
                Bitboard between = (fromBB << 8);

                if (two && (between & empty))
                {
                    int to2 = bb::lsb(two);
                    moves.push_back({(uint8_t)from, (uint8_t)to2, QUIET, 0});
                }
            }
        }
    }
}

void generateBlackPawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.blackPawns;

    Bitboard empty = ~(pos.whitePieces() | pos.blackPieces());

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        // -------------------------
        // single push
        // -------------------------
        Bitboard one = (fromBB >> 8) & empty;

        if (one)
        {
            int to = bb::lsb(one);
            moves.push_back({(uint8_t)from, (uint8_t)to, QUIET, 0});

            // -------------------------
            // double push (ONLY from rank 7)
            // -------------------------
            if (from / 8 == 6)
            {
                Bitboard two = (fromBB >> 16) & empty;

                Bitboard between = (fromBB >> 8);

                if (two && (between & empty))
                {
                    int to2 = bb::lsb(two);
                    moves.push_back({(uint8_t)from, (uint8_t)to2, QUIET, 0});
                }
            }
        }
    }
}
