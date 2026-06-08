#include "position_init.h"

Position PositionInit::startpos()
{
    Position pos{};

    // -------------------------
    // WHITE PIECES (rank 1 & 2)
    // -------------------------

    pos.whitePawns = 0;
    for (int i = 8; i < 16; i++)
        pos.whitePawns |= (1ULL << i);

    pos.whiteRooks   = (1ULL << 0) | (1ULL << 7);
    pos.whiteKnights = (1ULL << 1) | (1ULL << 6);
    pos.whiteBishops = (1ULL << 2) | (1ULL << 5);
    pos.whiteQueens  = (1ULL << 3);
    pos.whiteKing    = (1ULL << 4);

    // -------------------------
    // BLACK PIECES (rank 7 & 8)
    // -------------------------

    pos.blackPawns = 0;
    for (int i = 48; i < 56; i++)
        pos.blackPawns |= (1ULL << i);

    pos.blackRooks   = (1ULL << 56) | (1ULL << 63);
    pos.blackKnights = (1ULL << 57) | (1ULL << 62);
    pos.blackBishops = (1ULL << 58) | (1ULL << 61);
    pos.blackQueens  = (1ULL << 59);
    pos.blackKing    = (1ULL << 60);

    // -------------------------
    // GAME STATE
    // -------------------------

    pos.sideToMove = WHITE;

    pos.whiteKingMoved = false;
    pos.blackKingMoved = false;

    pos.whiteRookAMoved = false;
    pos.whiteRookHMoved = false;
    pos.blackRookAMoved = false;
    pos.blackRookHMoved = false;

    pos.enPassantSquare = -1;

    return pos;
}
