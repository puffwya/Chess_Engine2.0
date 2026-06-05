#include "position.h"

Bitboard Position::whitePieces() const
{
    return whitePawns
         | whiteKnights
         | whiteBishops
         | whiteRooks
         | whiteQueens
         | whiteKing;
}

Bitboard Position::blackPieces() const
{
    return blackPawns
         | blackKnights
         | blackBishops
         | blackRooks
         | blackQueens
         | blackKing;
}

Bitboard Position::occupancy() const
{
    return whitePieces() | blackPieces();
}
