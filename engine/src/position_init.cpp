#include "position_init.h"

Position PositionInit::startpos()
{
    Position pos{};

    pos.whitePawns   = 0x000000000000FF00ULL;
    pos.whiteKnights = 0x0000000000000042ULL;
    pos.whiteBishops = 0x0000000000000024ULL;
    pos.whiteRooks   = 0x0000000000000081ULL;
    pos.whiteQueens  = 0x0000000000000008ULL;
    pos.whiteKing    = 0x0000000000000010ULL;

    pos.blackPawns   = 0x00FF000000000000ULL;
    pos.blackKnights = 0x4200000000000000ULL;
    pos.blackBishops = 0x2400000000000000ULL;
    pos.blackRooks   = 0x8100000000000000ULL;
    pos.blackQueens  = 0x0800000000000000ULL;
    pos.blackKing    = 0x1000000000000000ULL;

    pos.sideToMove = WHITE;

    return pos;
}
