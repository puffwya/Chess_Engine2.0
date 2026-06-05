#include "movegen.h"
#include "attacks.h"
#include "position.h"
#include "bitboard.h"

void generateWhiteKingMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard king = pos.whiteKing;

    if (!king)
        return;

    int from = bb::lsb(king);

    Bitboard targets =
        kingAttacks[from] &
        ~pos.whitePieces();

    while (targets)
    {
        int to = bb::popLSB(targets);

        bool isCapture =
            (pos.blackPieces() & (1ULL << to));

        MoveFlags flags =
            isCapture ? CAPTURE : QUIET;

        moves.push_back(
            Move{
                static_cast<uint8_t>(from),
                static_cast<uint8_t>(to),
                flags,
                0
            }
        );
    }
}

void generateBlackKingMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard king = pos.blackKing;

    if (!king)
        return;

    int from = bb::lsb(king);

    Bitboard targets =
        kingAttacks[from] &
        ~pos.blackPieces();

    while (targets)
    {
        int to = bb::popLSB(targets);

        bool isCapture =
            (pos.whitePieces() & (1ULL << to));

        MoveFlags flags =
            isCapture ? CAPTURE : QUIET;

        moves.push_back(
            Move{
                static_cast<uint8_t>(from),
                static_cast<uint8_t>(to),
                flags,
                0
            }
        );
    }
}
