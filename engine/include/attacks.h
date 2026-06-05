#pragma once
#include "bitboard.h"

extern Bitboard knightAttacks[64];
extern Bitboard kingAttacks[64];

void initKnightAttacks();
void initKingAttacks();
