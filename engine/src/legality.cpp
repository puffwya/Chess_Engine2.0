#include "attack.h"
#include "legality.h"
#include "movegen.h"
#include "sliders.h"

// forward (you already effectively have this logic via sliders)
static void undoMoveInternal(Position& pos, const Move& move, uint8_t captured);

bool Legality::isKingInCheck(const Position& pos, Color side)
{
    int kingSq = (side == WHITE)
        ? bb::lsb(pos.whiteKing)
        : bb::lsb(pos.blackKing);

    return Attack::isSquareAttacked(pos, kingSq, side == WHITE ? BLACK : WHITE);
}

bool Legality::isMoveLegal(const Position& pos, const Move& move)
{
    Position copy = pos;

    Color us = pos.sideToMove;

    return !isKingInCheck(copy, us);
}

void Legality::filterLegalMoves(Position& pos, std::vector<Move>& moves)
{
    std::vector<Move> legal;
    legal.reserve(moves.size());

    for (const Move& m : moves)
    {
        if (isMoveLegal(pos, m))
            legal.push_back(m);
    }

    moves = std::move(legal);
}
