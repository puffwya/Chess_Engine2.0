#include "makemove.h"

// helper macros for bit ops
#define POP(bb, sq) (bb &= ~(1ULL << (sq)))
#define PUSH(bb, sq) (bb |=  (1ULL << (sq)))

void MoveMaker::makeMove(Position& pos, const Move& move, UndoInfo& undo)
{
    bool white = (pos.sideToMove == WHITE);

    undo.capturedPiece = 0;
    undo.enPassantSquare = pos.enPassantSquare;

    pos.enPassantSquare = -1;

    Bitboard fromBB = 1ULL << move.from;
    Bitboard toBB   = 1ULL << move.to;

    // Identify moving piece
    Bitboard* pieces[6] = {
        white ? &pos.whitePawns   : &pos.blackPawns,
        white ? &pos.whiteKnights : &pos.blackKnights,
        white ? &pos.whiteBishops : &pos.blackBishops,
        white ? &pos.whiteRooks   : &pos.blackRooks,
        white ? &pos.whiteQueens  : &pos.blackQueens,
        white ? &pos.whiteKing    : &pos.blackKing
    };

    // find which piece moved
    int movedIndex = -1;
    for (int i = 0; i < 6; i++)
    {
        if (*pieces[i] & fromBB)
        {
            movedIndex = i;
            POP(*pieces[i], move.from);
            break;
        }
    }

    if (move.flags == DOUBLE_PUSH)
    {
        if (white)
            pos.enPassantSquare = move.from + 8;
        else
            pos.enPassantSquare = move.from - 8;
    }

    // Handle captures
    Bitboard* opp[6] = {
        white ? &pos.blackPawns   : &pos.whitePawns,
        white ? &pos.blackKnights : &pos.whiteKnights,
        white ? &pos.blackBishops : &pos.whiteBishops,
        white ? &pos.blackRooks   : &pos.whiteRooks,
        white ? &pos.blackQueens  : &pos.whiteQueens,
        white ? &pos.blackKing    : &pos.whiteKing
    };

    if (move.flags == EN_PASSANT)
    {
        int capturedSq =
            white
                ? move.to - 8
                : move.to + 8;

        Bitboard capturedBB = 1ULL << capturedSq;

        if (white)
            pos.blackPawns &= ~capturedBB;
        else
            pos.whitePawns &= ~capturedBB;

        undo.capturedPiece = 1;
    }

    for (int i = 0; i < 6; i++)
    {
        if (*opp[i] & toBB)
        {
            POP(*opp[i], move.to);
            undo.capturedPiece = i + 1; // simple encoding
            break;
        }
    }

    // Move piece to destination
    if (movedIndex != -1)
    {
        // -------------------------
        // PROMOTION
        // -------------------------
        if (move.flags & PROMOTION)
        {
            if (white)
            {
                switch (move.promo)
                {
                    case QUEEN:  PUSH(pos.whiteQueens, move.to);  break;
                    case ROOK:   PUSH(pos.whiteRooks, move.to);   break;
                    case BISHOP: PUSH(pos.whiteBishops, move.to); break;
                    case KNIGHT: PUSH(pos.whiteKnights, move.to); break;
                }
            }
            else
            {
                switch (move.promo)
                {
                    case QUEEN:  PUSH(pos.blackQueens, move.to);  break;
                    case ROOK:   PUSH(pos.blackRooks, move.to);   break;
                    case BISHOP: PUSH(pos.blackBishops, move.to); break;
                    case KNIGHT: PUSH(pos.blackKnights, move.to); break;
                }
            }
        }
        else
        {
            // normal move (existing behavior)
            PUSH(*pieces[movedIndex], move.to);
        }
    }

    // -------------------------
    // Castling rook move
    // -------------------------
    if (move.flags == CASTLE)
    {
        if (white)
        {
            // White kingside
            if (move.to == 6)
            {
                POP(pos.whiteRooks, 7);
                PUSH(pos.whiteRooks, 5);
            }
            // White queenside
            else if (move.to == 2)
            {
                POP(pos.whiteRooks, 0);
                PUSH(pos.whiteRooks, 3);
            }
        }
        else
        {
            // Black kingside
            if (move.to == 62)
            {
                POP(pos.blackRooks, 63);
                PUSH(pos.blackRooks, 61);
            }
            // Black queenside
            else if (move.to == 58)
            {
                POP(pos.blackRooks, 56);
                PUSH(pos.blackRooks, 59);
            }
        }
    }

    undo.wasWhite = white;

    // -------------------------
    // update castling rights
    // -------------------------
    if (white)
    {
        if (movedIndex == 5 && !pos.whiteKingMoved) // king
            pos.whiteKingMoved = true;

        if (movedIndex == 3 && (!pos.whiteRookAMoved || !pos.whiteRookHMoved)) // rook
        {
            if (move.from == 0)  pos.whiteRookAMoved = true;
            if (move.from == 7)  pos.whiteRookHMoved = true;
        }
    }
    else
    {
        if (movedIndex == 5 && !pos.blackKingMoved)
            pos.blackKingMoved = true;

        if (movedIndex == 3 && (!pos.blackRookAMoved || !pos.blackRookHMoved))
        {
            if (move.from == 56) pos.blackRookAMoved = true;
            if (move.from == 63) pos.blackRookHMoved = true;
        }
    }

    // Side switch
    pos.sideToMove = white ? BLACK : WHITE;
}

void MoveMaker::unmakeMove(Position& pos, const Move& move, const UndoInfo& undo)
{
    bool white = (pos.sideToMove == WHITE);

    Bitboard fromBB = 1ULL << move.from;
    Bitboard toBB   = 1ULL << move.to;

    Bitboard* pieces[6] = {
        white ? &pos.whitePawns   : &pos.blackPawns,
        white ? &pos.whiteKnights : &pos.blackKnights,
        white ? &pos.whiteBishops : &pos.blackBishops,
        white ? &pos.whiteRooks   : &pos.blackRooks,
        white ? &pos.whiteQueens  : &pos.blackQueens,
        white ? &pos.whiteKing    : &pos.blackKing
    };

    Bitboard* opp[6] = {
        white ? &pos.blackPawns   : &pos.whitePawns,
        white ? &pos.blackKnights : &pos.whiteKnights,
        white ? &pos.blackBishops : &pos.whiteBishops,
        white ? &pos.blackRooks   : &pos.whiteRooks,
        white ? &pos.blackQueens  : &pos.whiteQueens,
        white ? &pos.blackKing    : &pos.whiteKing
    };

    // restore moved piece
    for (int i = 0; i < 6; i++)
    {
        if (*pieces[i] & toBB)
        {
            POP(*pieces[i], move.to);
            PUSH(*pieces[i], move.from);
            break;
        }
    }

    if (move.flags == EN_PASSANT)
    {
        int capturedSq =
            white
                ? move.to - 8
                : move.to + 8;

        PUSH(*opp[0], capturedSq);

        pos.sideToMove = white ? WHITE : BLACK;
        pos.enPassantSquare = undo.enPassantSquare;
        return;
    }

    // restore capture
    if (undo.capturedPiece != 0)
    {
        PUSH(*opp[undo.capturedPiece - 1], move.to);
    }

    white = undo.wasWhite;

    pos.enPassantSquare = undo.enPassantSquare;

    pos.sideToMove = white ? WHITE : BLACK;
}
