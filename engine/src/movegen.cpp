#include "movegen.h"
#include "movegen/pawn.h"
#include "movegen/knight.h"
#include "movegen/bishop.h"
#include "movegen/rook.h"
#include "movegen/queen.h"
#include "movegen/king.h"

void MoveGenerator::generateMoves(const Position& pos, std::vector<Move>& moves)
{
    moves.clear();

    if (pos.sideToMove == WHITE)
    {
        generateWhitePawnMoves(pos, moves);
        generateWhiteKnightMoves(pos, moves);
        generateWhiteBishopMoves(pos, moves);
        generateWhiteRookMoves(pos, moves);
        generateWhiteQueenMoves(pos, moves);
        generateWhiteKingMoves(pos, moves);
    }
    else
    {
        generateBlackPawnMoves(pos, moves);
        generateBlackKnightMoves(pos, moves);
        generateBlackBishopMoves(pos, moves);
        generateBlackRookMoves(pos, moves);
        generateBlackQueenMoves(pos, moves);
        generateBlackKingMoves(pos, moves);
    }
}
