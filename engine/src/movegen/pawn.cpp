#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "movegen/pawn.h"

void generateWhitePawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.whitePawns;

    Bitboard empty   = ~(pos.whitePieces() | pos.blackPieces());
    Bitboard enemies = pos.blackPieces();

    while (pawns)
    {
        int from = bb::popLSB(pawns);
        Bitboard fromBB = 1ULL << from;

        // =========================================================
        // SINGLE PUSH
        // =========================================================
        Bitboard one = (fromBB << 8) & empty;

        while (one)
        {
            int to = bb::popLSB(one);

            if (to / 8 == 7)
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
            }
            else
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, QUIET, NONE});
            }
        }

        // double push
        if (from / 8 == 1)
        {
            Bitboard two = (fromBB << 16) & empty;
            Bitboard between = (fromBB << 8);

            if (two && (between & empty))
            {
                int to2 = bb::lsb(two);
                moves.push_back(Move{(uint8_t)from, (uint8_t)to2, DOUBLE_PUSH, NONE});
            }
        }

        // =========================================================
        // CAPTURES
        // =========================================================

        // left capture
        Bitboard capLeft = (fromBB << 7) & enemies & ~FILE_H;

        while (capLeft)
        {
            int to = bb::popLSB(capLeft);

            if (to / 8 == 7)
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
            }
            else
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, CAPTURE, NONE});
            }
        }

        // right capture
        Bitboard capRight = (fromBB << 9) & enemies & ~FILE_A;

        while (capRight)
        {
            int to = bb::popLSB(capRight);

            if (to / 8 == 7)
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
            }
            else
            {
                moves.push_back(Move{(uint8_t)from, (uint8_t)to, CAPTURE, NONE});
            }
        }

        // =========================================================
        // EN PASSANT
        // =========================================================
        if (pos.enPassantSquare != -1)
        {
            if (from + 7 == pos.enPassantSquare && !(fromBB & FILE_A))
            {
                moves.push_back(Move{
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }

            if (from + 9 == pos.enPassantSquare && !(fromBB & FILE_H))
            {
                moves.push_back(Move{
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }
        }
    }
}

void generateBlackPawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.blackPawns;

    Bitboard empty   = ~(pos.whitePieces() | pos.blackPieces());
    Bitboard enemies = pos.whitePieces();

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        // -------------------------
        // single push (down)
        // -------------------------
        Bitboard one = (fromBB >> 8) & empty;

        if (one)
        {
            Bitboard targets = one;

            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 0)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({(uint8_t)from, (uint8_t)to, QUIET, NONE});
                }
            }

            // double push (rank 7 start)
            if (from / 8 == 6)
            {
                Bitboard two = (fromBB >> 16) & empty;
                Bitboard between = (fromBB >> 8) & empty;

                if (two && between)
                {
                    int to2 = bb::lsb(two);
                    moves.push_back({(uint8_t)from, (uint8_t)to2, DOUBLE_PUSH, NONE});
                }
            }
        }

        // -------------------------
        // captures
        // -------------------------

        // down-left
        Bitboard capLeft = (fromBB >> 9) & enemies & ~FILE_H;
        if (capLeft)
        {
            Bitboard targets = capLeft;

            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 0)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({(uint8_t)from, (uint8_t)to, CAPTURE, NONE});
                }
            }
        }

        // down-right
        Bitboard capRight = (fromBB >> 7) & enemies & ~FILE_A;
        if (capRight)
        {
            Bitboard targets = capRight;

            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 0)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({(uint8_t)from, (uint8_t)to, CAPTURE, NONE});
                }
            }
        }

        // -------------------------
        // en passant
        // -------------------------
        if (pos.enPassantSquare != -1)
        {
            if (from - 7 == pos.enPassantSquare && (fromBB & ~FILE_H))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }

            if (from - 9 == pos.enPassantSquare && (fromBB & ~FILE_A))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }
        }
    }
}
