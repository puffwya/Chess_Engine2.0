#pragma once
#include <cstdint>

using Bitboard = uint64_t;

namespace bb
{
    int popLSB(Bitboard& b);
    int lsb(Bitboard b);
    void print(Bitboard b);
}
