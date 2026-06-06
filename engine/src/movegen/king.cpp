#include "movegen.h"
#include "attack.h" 
#include "attacks.h"
#include "position.h"
#include "bitboard.h"
#include "movegen/king.h"

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
                NONE
            }
        );
    }

    // -------------------------
    // White kingside castle
    // -------------------------
    if (!pos.whiteKingMoved &&
        !pos.whiteRookHMoved)
    {
        bool empty =
            !(pos.occupancy() &
            ((1ULL << 5) | (1ULL << 6)));

        if (empty)
        {
            if (!Attack::isSquareAttacked(pos, 4, BLACK) &&
                !Attack::isSquareAttacked(pos, 5, BLACK) &&
                !Attack::isSquareAttacked(pos, 6, BLACK))
            {
                moves.push_back(
                    Move{
                        4,
                        6,
                        CASTLE,
                        NONE
                    }
                );
            }
        }
    }

    // -------------------------
    // White queenside castle
    // -------------------------
    if (!pos.whiteKingMoved &&
        !pos.whiteRookAMoved)
    {
        bool empty =
            !(pos.occupancy() &
            ((1ULL << 1) |
             (1ULL << 2) |
             (1ULL << 3)));

        if (empty)
        {
            if (!Attack::isSquareAttacked(pos, 4, BLACK) &&
                !Attack::isSquareAttacked(pos, 3, BLACK) &&
                !Attack::isSquareAttacked(pos, 2, BLACK))
            {
                moves.push_back(
                    Move{
                        4,
                        2,
                        CASTLE,
                        NONE
                    }
                );
            }
        }
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
                NONE
            }
        );
    }

    // -------------------------
    // Black kingside castle
    // -------------------------
    if (!pos.blackKingMoved &&
        !pos.blackRookHMoved)
    {
        bool empty =
            !(pos.occupancy() &
            ((1ULL << 61) | (1ULL << 62)));

        if (empty)
        {
            if (!Attack::isSquareAttacked(pos, 60, WHITE) &&
                !Attack::isSquareAttacked(pos, 61, WHITE) &&
                !Attack::isSquareAttacked(pos, 62, WHITE))
            {
                moves.push_back(
                    Move{
                        60,
                        62,
                        CASTLE,
                        NONE
                    }
                );
            }
        }
    }

    // -------------------------
    // Black queenside castle
    // -------------------------
    if (!pos.blackKingMoved &&
        !pos.blackRookAMoved)
    {
        bool empty =
            !(pos.occupancy() &
            ((1ULL << 57) |
             (1ULL << 58) |
             (1ULL << 59)));

        if (empty)
        {
            if (!Attack::isSquareAttacked(pos, 60, WHITE) &&
                !Attack::isSquareAttacked(pos, 59, WHITE) &&
                !Attack::isSquareAttacked(pos, 58, WHITE))
            {
                moves.push_back(
                    Move{
                        60,
                        58,
                        CASTLE,
                        NONE
                    }
                );
            }
        }
    }
}
