#pragma once
#include "bitboard.h"

enum Color { WHITE, BLACK };

struct Position
{
    Bitboard whitePawns;
    Bitboard whiteKnights;
    Bitboard whiteBishops;
    Bitboard whiteRooks;
    Bitboard whiteQueens;
    Bitboard whiteKing;

    Bitboard blackPawns;
    Bitboard blackKnights;
    Bitboard blackBishops;
    Bitboard blackRooks;
    Bitboard blackQueens;
    Bitboard blackKing;

    Bitboard whitePieces() const;
    Bitboard blackPieces() const;
    Bitboard occupancy() const;

    Color sideToMove;
};
