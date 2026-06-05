#pragma once
#include <cstdint>

using Bitboard = uint64_t;

enum Color
{
    WHITE,
    BLACK
};

enum PieceType
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};
