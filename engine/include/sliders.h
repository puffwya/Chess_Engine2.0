#pragma once
#include <cstdint>
#include "bitboard.h"

// directions are encoded as (fileDelta, rankDelta)
struct Direction {
    int df;
    int dr;
};

Bitboard slidingAttacks(int square, const Direction* dirs, int dirCount, Bitboard occupancy);
