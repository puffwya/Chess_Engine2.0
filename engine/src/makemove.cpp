#include "makemove.h"

// helper macros for bit ops
#define POP(bb, sq) (bb &= ~(1ULL << (sq)))
#define PUSH(bb, sq) (bb |=  (1ULL << (sq)))

void MoveMaker::makeMove(Position& pos, const Move& move, UndoInfo& undo)
{
    undo.capturedPiece = 0;
    undo.enPassantSquare = -1;

    Bitboard fromBB = 1ULL << move.from;
    Bitboard toBB   = 1ULL << move.to;

    bool white = (pos.sideToMove == WHITE);

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

    // Handle captures
    Bitboard* opp[6] = {
        white ? &pos.blackPawns   : &pos.whitePawns,
        white ? &pos.blackKnights : &pos.whiteKnights,
        white ? &pos.blackBishops : &pos.whiteBishops,
        white ? &pos.blackRooks   : &pos.whiteRooks,
        white ? &pos.blackQueens  : &pos.whiteQueens,
        white ? &pos.blackKing    : &pos.whiteKing
    };

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
        PUSH(*pieces[movedIndex], move.to);
    }

    // Side switch
    pos.sideToMove = white ? BLACK : WHITE;
}

void MoveMaker::unmakeMove(Position& pos, const Move& move, const UndoInfo& undo)
{
    bool white = (pos.sideToMove == BLACK); // reversed turn

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

    // restore capture
    if (undo.capturedPiece != 0)
    {
        PUSH(*opp[undo.capturedPiece - 1], move.to);
    }

    pos.sideToMove = white ? WHITE : BLACK;
}
