#include "perft.h"
#include "movegen.h"
#include "makemove.h"
#include "attack.h"
#include <iostream>

static long long perftInternal(Position& pos, int depth)
{
    if (depth == 0)
        return 1;

    std::vector<Move> moves;
    MoveGenerator::generateMoves(pos, moves);

    long long nodes = 0;

    for (const Move& move : moves)
    {
        UndoInfo undo;
        MoveMaker::makeMove(pos, move, undo);

        // IMPORTANT: legality check via king safety
        int kingSq =
            bb::lsb(pos.sideToMove == WHITE
                ? pos.blackKing
                : pos.whiteKing);

        bool inCheck = Attack::isSquareAttacked(
            pos,
            kingSq,
            pos.sideToMove
        );

        if (!inCheck)
            nodes += perftInternal(pos, depth - 1);

        MoveMaker::unmakeMove(pos, move, undo);
    }

    return nodes;
}

long long Perft::perft(Position& pos, int depth)
{
    return perftInternal(pos, depth);
}

void Perft::perftDivide(Position& pos, int depth)
{
    std::vector<Move> moves;
    MoveGenerator::generateMoves(pos, moves);

    long long total = 0;

    for (const Move& move : moves)
    {
        UndoInfo undo;
        MoveMaker::makeMove(pos, move, undo);

        int kingSq =
            bb::lsb(pos.sideToMove == WHITE
                ? pos.blackKing
                : pos.whiteKing);

        bool inCheck = Attack::isSquareAttacked(
            pos,
            kingSq,
            pos.sideToMove
        );

        long long nodes = 0;

        if (!inCheck)
            nodes = perftInternal(pos, depth - 1);

        MoveMaker::unmakeMove(pos, move, undo);

        std::cout
            << "Move "
            << (int)move.from << "->" << (int)move.to
            << " : " << nodes << "\n";

        total += nodes;
    }

    std::cout << "Total nodes: " << total << "\n";
}
