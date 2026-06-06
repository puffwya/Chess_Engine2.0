#include <vector>
#include "position.h"
#include "move.h"
#include "attacks.h"
#include "movegen/knight.h"

void generateWhiteKnightMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard knights = pos.whiteKnights;

    Bitboard friendly =
        pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
        pos.whiteRooks | pos.whiteQueens | pos.whiteKing;

    Bitboard enemy =
        pos.blackPawns | pos.blackKnights | pos.blackBishops |
        pos.blackRooks | pos.blackQueens | pos.blackKing;

    Bitboard p = knights;

    while (p)
    {
        int from = bb::popLSB(p);

        Bitboard attacks = knightAttacks[from] & ~friendly;

        Bitboard temp = attacks;
        while (temp)
        {
            int to = bb::popLSB(temp);

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

void generateBlackKnightMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard knights = pos.blackKnights;

    Bitboard friendly =
        pos.blackPawns | pos.blackKnights | pos.blackBishops |
        pos.blackRooks | pos.blackQueens | pos.blackKing;

    Bitboard enemy =
        pos.whitePawns | pos.whiteKnights | pos.whiteBishops |
        pos.whiteRooks | pos.whiteQueens | pos.whiteKing;

    Bitboard p = knights;

    while (p)
    {
        int from = bb::popLSB(p);

        Bitboard attacks = knightAttacks[from] & ~friendly;

        Bitboard temp = attacks;
        while (temp)
        {
            int to = bb::popLSB(temp);

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
