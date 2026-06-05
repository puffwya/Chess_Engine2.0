#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"

static const Bitboard FILE_A = 0x0101010101010101ULL;
static const Bitboard FILE_H = 0x8080808080808080ULL;

void generateWhitePawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.whitePawns;

    Bitboard empty = ~(pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
                       pos.whiteRooks | pos.whiteQueens | pos.whiteKing |
                       pos.blackPawns | pos.blackKnights | pos.blackBishops |
                       pos.blackRooks | pos.blackQueens | pos.blackKing);

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        Bitboard one = (fromBB << 8) & empty;

        if (one)
        {
            int to = bb::lsb(one);
            moves.push_back({(uint8_t)from, (uint8_t)to, QUIET, 0});
        }
    }
}

void generateBlackPawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.blackPawns;

    Bitboard empty = ~(pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
                       pos.whiteRooks | pos.whiteQueens | pos.whiteKing |
                       pos.blackPawns | pos.blackKnights | pos.blackBishops |
                       pos.blackRooks | pos.blackQueens | pos.blackKing);

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        Bitboard one = (fromBB >> 8) & empty;

        if (one)
        {
            int to = bb::lsb(one);
            moves.push_back({(uint8_t)from, (uint8_t)to, QUIET, 0});
        }
    }
}
