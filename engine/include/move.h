#pragma once
#include <cstdint>

enum MoveFlags : uint8_t
{
    QUIET = 0,
    CAPTURE = 1,
    DOUBLE_PUSH = 2,
    PROMOTION = 4,
    EN_PASSANT = 8
};

struct Move
{
    uint8_t from;
    uint8_t to;
    MoveFlags flags;
    uint8_t promo;
};
