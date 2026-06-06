#pragma once
#include <cstdint>

enum MoveFlags : uint8_t
{
    QUIET = 0,
    CAPTURE = 1,
    DOUBLE_PUSH = 2,
    PROMOTION = 4,
    EN_PASSANT = 8,
    CASTLE = 16
};

enum PieceType : uint8_t
{
    NONE   = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK   = 3,
    QUEEN  = 4
};

struct Move
{
    uint8_t from;
    uint8_t to;
    MoveFlags flags;
    PieceType promo;
};
